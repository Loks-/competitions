#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/at.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/find.h"
#include "common/binary_search_tree/base/insert_by_key.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/remove_push_down.h"
#include "common/binary_search_tree/base/remove_right.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/self_balancing_tree.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/rbt_color.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <tuple>
#include <vector>

namespace bst {

template <bool has_key, bool has_parent, typename Data,
          typename AggregatorsTuple = std::tuple<subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t>
class RedBlackTree
    : public base::SelfBalancingTree<
          memory::ContiguousNodesManager<base::Node<
              Data,
              base::SubtreeData<templates::PrependT<subtree_data::RBTColor,
                                                    AggregatorsTuple>>,
              base::Deferred<DeferredTuple>, has_parent, has_key, Key>>,
          RedBlackTree<has_key, has_parent, Data, AggregatorsTuple,
                       DeferredTuple, Key>> {
 public:
  using SubtreeDataType = base::SubtreeData<
      templates::PrependT<subtree_data::RBTColor, AggregatorsTuple>>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, has_parent, has_key, Key>;
  using Self = RedBlackTree<has_key, has_parent, Data, AggregatorsTuple,
                            DeferredTuple, Key>;
  using SBTree =
      base::SelfBalancingTree<memory::ContiguousNodesManager<NodeType>, Self>;
  using Base = typename SBTree::Base;

  friend Base;
  friend SBTree;

 protected:
  [[nodiscard]] static constexpr bool is_black(const NodeType* node) {
    return !node || subtree_data::RBTColor::get(node);
  }

  [[nodiscard]] static constexpr bool is_red(const NodeType* node) {
    return !is_black(node);
  }

  static constexpr void set_color(NodeType* node, bool black) {
    assert(node);
    subtree_data::RBTColor::set(node, black);
  }

  static constexpr void set_black(NodeType* node) { set_color(node, true); }

  static constexpr void set_red(NodeType* node) { set_color(node, false); }

 public:
  explicit RedBlackTree(size_t max_nodes) : SBTree(max_nodes) {}

 protected:
  static void BuildTreeIFixColorsR(NodeType* root, size_t height) {
    assert(root || !height);
    if (!root) return;
    if (height) {
      set_color(root, true);
      BuildTreeIFixColorsR(root->left, height - 1);
      BuildTreeIFixColorsR(root->right, height - 1);
    } else {
      set_color(root, false);
      assert(!root->left && !root->right);
    }
  }

  template <bool update_leafs>
  static NodeType* build_tree_impl(const std::vector<NodeType*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    size_t h = 0;
    for (; nodes.size() >= (1ull << h);) ++h;
    NodeType* root = Base::template build_tree_impl<update_leafs>(nodes);
    BuildTreeIFixColorsR(root, h - 1);
    set_color(root, true);
    return root;
  }

  static NodeType* insert_impl_base(NodeType* root, NodeType* node) {
    for (;;) {
      NodeType* parent = node->parent;
      if (!parent) {
        set_color(node, true);
        return node;
      }
      if (is_black(parent)) return root;
      NodeType* gparent = parent->parent;
      NodeType* uncle = base::sibling(parent, gparent);
      if (is_black(uncle)) {
        const bool rotate_required =
            ((gparent->left == parent) != (parent->left == node));
        if (rotate_required) {
          base::rotate_up<false, false>(node);
          parent = node;
        }
        base::rotate_up<true, false>(parent);
        set_color(gparent, false);
        set_color(parent, true);
        return parent->parent ? root : parent;
      }
      set_color(parent, true);
      set_color(uncle, true);
      set_color(gparent, false);
      node = gparent;
    }
    assert(false);
    return nullptr;
  }

  template <bool update_required>
  static NodeType* insert_impl(NodeType* root, NodeType* node) {
    assert(node);
    if constexpr (update_required) node->update_subtree_data();
    if (!root) {
      set_black(node);
      return node;
    }
    base::InsertByKey<NodeType>(root, node);
    set_red(node);
    return insert_impl_base(root, node);
  }

  template <bool update_required>
  static NodeType* insert_at_impl(NodeType* root, NodeType* node,
                                  size_t index) {
    assert(node);
    if constexpr (update_required) node->update_subtree_data();
    if (!root) {
      assert(index == 0);
      set_black(node);
      return node;
    }
    const auto old_root = root;
    root->apply_deferred();
    for (;;) {
      const size_t lsize = bst::subtree_data::size(root->left);
      if (index <= lsize) {
        if (!root->left) {
          assert(index == 0);
          root->set_left(node);
          break;
        }
        root = root->left;
      } else {
        if (!root->right) {
          assert(index == lsize + 1);
          root->set_right(node);
          break;
        }
        root = root->right;
        index -= lsize + 1;
      }
      root->apply_deferred();
    }
    assert(root);
    set_red(node);
    subtree_data::propagate_to_root(root);
    return insert_impl_base(old_root, node);
  }

 protected:
  template <bool reset_links>
  static NodeType* remove_impl(NodeType* root, const Key& key,
                               NodeType*& removed_node) {
    removed_node = base::find(root, key);
    return (removed_node ? remove_node_impl<reset_links>(removed_node) : root);
  }

  template <bool reset_links>
  static NodeType* remove_at_impl(NodeType* root, size_t index,
                                  NodeType*& removed_node) {
    removed_node = base::at(root, index);
    return (removed_node ? remove_node_impl<reset_links>(removed_node) : root);
  }

  template <bool reset_links>
  static NodeType* remove_node_impl(NodeType* node) {
    base::RemovePushDown<NodeType, false>(node);
    const bool black = is_black(node);

    // Drop node from tree
    NodeType* child = node->left ? node->left : node->right;
    NodeType* parent = node->parent;
    if (parent) {
      if (parent->left == node)
        parent->left = child;
      else
        parent->right = child;
    }
    if (child) child->parent = parent;
    if constexpr (reset_links) node->reset_links_and_update_subtree_data();
    subtree_data::propagate_to_root(parent);

    // Fix colors
    if (!black) return (parent ? base::root(parent) : child);
    for (;;) {
      if (is_red(child)) {
        set_color(child, true);
        return base::root(child);
      }
      if (!parent) return child;
      NodeType* sibling = base::sibling(child, parent);
      assert(sibling);
      sibling->apply_deferred();
      if (is_red(sibling)) {
        assert(is_black(parent));
        base::rotate_up<true, false>(sibling);
        set_color(sibling, true);
        set_color(parent, false);
        sibling = base::sibling(child, parent);
        sibling->apply_deferred();
      }
      assert(sibling && is_black(sibling));
      if (is_black(parent) && is_black(sibling->left) &&
          is_black(sibling->right)) {
        set_color(sibling, false);
        child = parent;
        parent = child->parent;
        continue;
      }
      if (is_red(parent) && is_black(sibling->left) &&
          is_black(sibling->right)) {
        set_color(sibling, false);
        set_color(parent, true);
        return base::root(parent);
      }
      if ((parent->left == child) && is_black(sibling->right)) {
        assert(is_red(sibling->left));
        base::rotate_up<false, true>(sibling->left);
        set_color(sibling, false);
        sibling = sibling->parent;
        set_color(sibling, true);
      } else if ((parent->right == child) && is_black(sibling->left)) {
        assert(is_red(sibling->right));
        base::rotate_up<false, true>(sibling->right);
        set_color(sibling, false);
        sibling = sibling->parent;
        set_color(sibling, true);
      }
      set_color(sibling, is_black(parent));
      set_color(parent, true);
      if (parent->left == child) {
        set_color(sibling->right, true);
      } else {
        set_color(sibling->left, true);
      }
      base::rotate_up<true, false>(sibling);
      return base::root(sibling);
    }
    assert(false);
    return nullptr;
  }

  template <bool reset_links>
  static NodeType* remove_right_impl(NodeType* root, NodeType*& removed_node) {
    for (root->apply_deferred(); root->right; root->apply_deferred())
      root = root->right;
    removed_node = root;
    return remove_node_impl<reset_links>(removed_node);
  }

  [[nodiscard]] static constexpr int black_height(NodeType* root) {
    int h = 0;
    for (; root; root = root->left) {
      root->apply_deferred();
      if (is_black(root)) ++h;
    }
    return h;
  }

  [[nodiscard]] static constexpr NodeType* join3_base_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r) {
    SBTree::join3_base_impl(l, m1, r);
    set_red(m1);
    return m1;
  }

  [[nodiscard]] static constexpr NodeType* join3_left_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r,
                                                           int hd) {
    if (is_black(l) && (hd == 0)) return join3_base_impl(l, m1, r);
    l->apply_deferred();
    l->set_right(join3_left_impl(l->right, m1, r, hd - (is_black(l) ? 1 : 0)));
    r = l->right;
    if (is_black(l) && is_red(r) && is_red(r->right)) {
      // Rotate left
      set_black(r->right);
      l->set_right(r->left);
      l->update_subtree_data();
      r->set_left(l);
      r->update_subtree_data();
      return r;
    } else {
      l->update_subtree_data();
      return l;
    }
  }

  [[nodiscard]] static constexpr NodeType* join3_right_impl(NodeType* l,
                                                            NodeType* m1,
                                                            NodeType* r,
                                                            int hd) {
    if (is_black(r) && (hd == 0)) return join3_base_impl(l, m1, r);
    r->apply_deferred();
    r->set_left(join3_right_impl(l, m1, r->left, hd - (is_black(r) ? 1 : 0)));
    l = r->left;
    if (is_black(r) && is_red(l) && is_red(l->left)) {
      // Rotate right
      set_black(l->left);
      r->set_left(l->right);
      r->update_subtree_data();
      l->set_right(r);
      l->update_subtree_data();
      return l;
    } else {
      r->update_subtree_data();
      return r;
    }
  }

 public:
  [[nodiscard]] static constexpr NodeType* join3_impl(NodeType* l, NodeType* m1,
                                                      NodeType* r) {
    const auto hl = black_height(l), hr = black_height(r), hd = hl - hr;
    auto root = (hd > 0)   ? join3_left_impl(l, m1, r, hd)
                : (hd < 0) ? join3_right_impl(l, m1, r, -hd)
                           : join3_base_impl(l, m1, r);
    set_black(root);
    root->set_parent(nullptr);
    return root;
  }
};
}  // namespace bst
