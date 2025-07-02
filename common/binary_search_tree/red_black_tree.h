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
  static constexpr bool IsBlack(const NodeType* node) {
    return !node || subtree_data::RBTColor::get(node);
  }

  static constexpr bool IsRed(const NodeType* node) {
    return node && !subtree_data::RBTColor::get(node);
  }

  static constexpr void SetColor(NodeType* node, bool is_black) {
    assert(node);
    subtree_data::RBTColor::set(node, is_black);
  }

 public:
  explicit RedBlackTree(size_t max_nodes) : SBTree(max_nodes) {}

 protected:
  static void BuildTreeIFixColorsR(NodeType* root, size_t height) {
    assert(root || !height);
    if (!root) return;
    if (height) {
      SetColor(root, true);
      BuildTreeIFixColorsR(root->left, height - 1);
      BuildTreeIFixColorsR(root->right, height - 1);
    } else {
      SetColor(root, false);
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
    SetColor(root, true);
    return root;
  }

  template <bool update_required>
  static NodeType* insert_impl(NodeType* root, NodeType* node) {
    assert(node);
    if constexpr (update_required) node->update_subtree_data();
    if (!root) {
      SetColor(node, true);
      return node;
    }
    base::InsertByKey<NodeType>(root, node);
    SetColor(node, false);
    for (;;) {
      NodeType* parent = node->parent;
      if (!parent) {
        SetColor(node, true);
        return node;
      }
      if (IsBlack(parent)) return root;
      NodeType* gparent = parent->parent;
      NodeType* uncle = base::sibling(parent, gparent);
      if (IsBlack(uncle)) {
        const bool rotate_required =
            ((gparent->left == parent) != (parent->left == node));
        if (rotate_required) {
          base::rotate_up<false, false>(node);
          parent = node;
        }
        base::rotate_up<true, false>(parent);
        SetColor(gparent, false);
        SetColor(parent, true);
        return parent->parent ? root : parent;
      }
      SetColor(parent, true);
      SetColor(uncle, true);
      SetColor(gparent, false);
      node = gparent;
    }
    assert(false);
    return nullptr;
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
    const bool black = IsBlack(node);

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
      if (IsRed(child)) {
        SetColor(child, true);
        return base::root(child);
      }
      if (!parent) return child;
      NodeType* sibling = base::sibling(child, parent);
      assert(sibling);
      sibling->apply_deferred();
      if (!IsBlack(sibling)) {
        assert(IsBlack(parent));
        base::rotate_up<true, false>(sibling);
        SetColor(sibling, true);
        SetColor(parent, false);
        sibling = base::sibling(child, parent);
        sibling->apply_deferred();
      }
      assert(sibling && IsBlack(sibling));
      if (IsBlack(parent) && IsBlack(sibling->left) &&
          IsBlack(sibling->right)) {
        SetColor(sibling, false);
        child = parent;
        parent = child->parent;
        continue;
      }
      if (!IsBlack(parent) && IsBlack(sibling->left) &&
          IsBlack(sibling->right)) {
        SetColor(sibling, false);
        SetColor(parent, true);
        return base::root(parent);
      }
      if ((parent->left == child) && IsBlack(sibling->right)) {
        assert(IsRed(sibling->left));
        base::rotate_up<false, true>(sibling->left);
        SetColor(sibling, false);
        sibling = sibling->parent;
        SetColor(sibling, true);
      } else if ((parent->right == child) && IsBlack(sibling->left)) {
        assert(IsRed(sibling->right));
        base::rotate_up<false, true>(sibling->right);
        SetColor(sibling, false);
        sibling = sibling->parent;
        SetColor(sibling, true);
      }
      SetColor(sibling, IsBlack(parent));
      SetColor(parent, true);
      if (parent->left == child) {
        SetColor(sibling->right, true);
      } else {
        SetColor(sibling->left, true);
      }
      base::rotate_up<true, false>(sibling);
      return base::root(sibling);
    }
    assert(false);
    return nullptr;
  }

 public:
  static NodeType* RemoveRight(NodeType* root, NodeType*& removed_node) {
    assert(root);
    auto node = root;
    for (node->apply_deferred(); node->right; node->apply_deferred())
      node = node->right;
    removed_node = node;
    return remove_node_impl<true>(removed_node);
  }

  static NodeType* join(NodeType* l, NodeType* r) {
    if (!l) return r;
    if (!r) return l;
    NodeType* node = nullptr;
    l = RemoveRight(l, node);
    return join3_impl(l, node, r);
  }

 protected:
  [[nodiscard]] static constexpr int black_height(NodeType* root) {
    int h = 0;
    for (; root; root = root->left) {
      root->apply_deferred();
      if (IsBlack(root)) ++h;
    }
    return h;
  }

  [[nodiscard]] static constexpr NodeType* join3_base_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r) {
    SBTree::join3_base_impl(l, m1, r);
    SetColor(m1, false);
    return m1;
  }

  [[nodiscard]] static constexpr NodeType* join3_left_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r,
                                                           int hd) {
    if (IsBlack(l) && (hd == 0)) return join3_base_impl(l, m1, r);
    l->apply_deferred();
    l->set_right(join3_left_impl(l->right, m1, r, hd - (IsBlack(l) ? 1 : 0)));
    r = l->right;
    if (IsBlack(l) && !IsBlack(r) && IsRed(r->right)) {
      SetColor(r->right, true);
      base::rotate<true, false, NodeType>(r, l, l->parent);
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
    if (IsBlack(r) && (hd == 0)) return join3_base_impl(l, m1, r);
    r->apply_deferred();
    r->set_left(join3_right_impl(l, m1, r->left, hd - (IsBlack(r) ? 1 : 0)));
    l = r->left;
    if (IsBlack(r) && !IsBlack(l) && IsRed(l->left)) {
      SetColor(l->left, true);
      base::rotate<true, false, NodeType>(l, r, r->parent);
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
    SetColor(root, true);
    return root;
  }
};
}  // namespace bst
