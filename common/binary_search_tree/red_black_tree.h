#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/at.h"
#include "common/binary_search_tree/base/base_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/find.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/push_down.h"
#include "common/binary_search_tree/base/remove_right.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/rbt_color.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_and_find_root.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root_with_path.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>

namespace bst {

template <bool has_key, bool has_parent, typename Data,
          typename AggregatorsTuple = std::tuple<subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t>
class RedBlackTree
    : public base::BaseTree<
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
  using Base = base::BaseTree<memory::ContiguousNodesManager<NodeType>, Self>;

  friend Base;

 public:
  explicit RedBlackTree(size_t max_nodes) : Base(max_nodes) {}

  [[nodiscard]] static constexpr NodeType* remove_right(
      NodeType* root, NodeType*& removed_node) {
    assert(root);
    return remove_right_impl<true>(root, removed_node);
  }

 protected:
  [[nodiscard]] static constexpr bool is_black(const NodeType* node) {
    return !node || subtree_data::RBTColor::get(node);
  }

  [[nodiscard]] static constexpr bool is_red(const NodeType* node) {
    return !is_black(node);
  }

  [[nodiscard]] static constexpr int black_height(NodeType* root) {
    int h = 0;
    for (; root; root = root->left) {
      root->apply_deferred();
      if (is_black(root)) ++h;
    }
    return h;
  }

  static constexpr void set_color(NodeType* node, bool black) {
    assert(node);
    subtree_data::RBTColor::set(node, black);
  }

  static constexpr void set_black(NodeType* node) { set_color(node, true); }

  static constexpr void set_red(NodeType* node) { set_color(node, false); }

  static constexpr void copy_color(NodeType* node_to,
                                   const NodeType* node_from) {
    set_color(node_to, is_black(node_from));
  }

  static constexpr void build_tree_impl_fix_colors(NodeType* root,
                                                   size_t height) {
    assert(root || !height);
    if (!root) return;
    if (height) {
      set_black(root);
      build_tree_impl_fix_colors(root->left, height - 1);
      build_tree_impl_fix_colors(root->right, height - 1);
    } else {
      set_red(root);
      assert(!root->left && !root->right);
    }
  }

  template <bool update_leafs>
  [[nodiscard]] static constexpr NodeType* build_tree_impl(
      const std::vector<NodeType*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    size_t h = 0;
    for (; nodes.size() >= (1ull << h);) ++h;
    NodeType* root = Base::template build_tree_impl<update_leafs>(nodes);
    build_tree_impl_fix_colors(root, h - 1);
    set_black(root);
    return root;
  }

  [[nodiscard]] static constexpr NodeType* insert_impl_internal_hpf(
      const std::vector<NodeType*>& nodes, bool update_required) {
    unsigned index = 0;
    while (index + 1 < nodes.size()) {
      NodeType* node = nodes[index];
      NodeType* parent = nodes[index + 1];
      if (is_black(parent)) {
        subtree_data::propagate_to_root_with_path(
            nodes, update_required ? index : index + 1);
        return nodes.back();
      }
      NodeType* grandparent = nodes[index + 2];
      NodeType* uncle = base::sibling(parent, grandparent);
      if (is_black(uncle)) {
        const bool rotate_required =
            ((grandparent->left == parent) != (parent->left == node));
        if (rotate_required) {
          base::rotate<false, false>(node, parent, grandparent);
          parent = node;
        } else if (update_required) {
          node->update_subtree_data();
        }
        base::rotate<true, false>(
            parent, grandparent,
            ((index + 3 < nodes.size()) ? nodes[index + 3] : nullptr));
        set_red(grandparent);
        set_black(parent);
        if (index + 3 < nodes.size()) {
          subtree_data::propagate_to_root_with_path(nodes, index + 3);
          return nodes.back();
        } else {
          return parent;
        }
      }
      if (update_required) node->update_subtree_data();
      set_black(parent);
      set_black(uncle);
      set_red(grandparent);
      parent->update_subtree_data();
      update_required = true;
      index += 2;
    }
    assert(index + 1 == nodes.size());
    NodeType* node = nodes.back();
    set_black(node);
    if (update_required) node->update_subtree_data();
    return node;
  }

  [[nodiscard]] static constexpr NodeType* insert_impl_internal_hpt(
      NodeType* node, bool update_required) {
    static_assert(has_parent, "has_parent should be true");
    while (NodeType* parent = node->parent) {
      if (is_black(parent))
        return subtree_data::propagate_and_find_root(update_required ? node
                                                                     : parent);
      NodeType* grandparent = parent->parent;
      NodeType* uncle = base::sibling(parent, grandparent);
      if (is_black(uncle)) {
        const bool rotate_required =
            ((grandparent->left == parent) != (parent->left == node));
        if (rotate_required) {
          base::rotate<false, false>(node, parent, grandparent);
          parent = node;
        } else if (update_required) {
          node->update_subtree_data();
        }
        base::rotate<true, false>(parent, grandparent, grandparent->parent);
        set_red(grandparent);
        set_black(parent);
        return parent->parent
                   ? subtree_data::propagate_and_find_root(parent->parent)
                   : parent;
      }
      if (update_required) node->update_subtree_data();
      set_black(parent);
      set_black(uncle);
      set_red(grandparent);
      parent->update_subtree_data();
      update_required = true;
      node = grandparent;
    }
    set_black(node);
    if (update_required) node->update_subtree_data();
    return node;
  }

  template <bool update_required>
  [[nodiscard]] static constexpr NodeType* insert_impl(NodeType* root,
                                                       NodeType* node) {
    if (!root) {
      set_black(node);
      if constexpr (update_required) node->update_subtree_data();
      return node;
    }
    std::vector<NodeType*> nodes;
    while (true) {
      root->apply_deferred();
      if constexpr (!has_parent) nodes.push_back(root);
      if (root->key < node->key) {
        if (!root->right) {
          root->set_right(node);
          break;
        }
        root = root->right;
      } else {
        if (!root->left) {
          root->set_left(node);
          break;
        }
        root = root->left;
      }
    }
    set_red(node);

    if constexpr (has_parent) {
      return insert_impl_internal_hpt(node, update_required);
    } else {
      nodes.push_back(node);
      std::reverse(nodes.begin(), nodes.end());
      return insert_impl_internal_hpf(nodes, update_required);
    }
  }

  template <bool update_required>
  [[nodiscard]] static constexpr NodeType* insert_at_impl(NodeType* root,
                                                          NodeType* node,
                                                          size_t index) {
    if (!root) {
      assert(index == 0);
      set_black(node);
      if constexpr (update_required) node->update_subtree_data();
      return node;
    }
    std::vector<NodeType*> nodes;
    while (true) {
      root->apply_deferred();
      if constexpr (!has_parent) nodes.push_back(root);
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
    }
    set_red(node);

    if constexpr (has_parent) {
      return insert_impl_internal_hpt(node, update_required);
    } else {
      nodes.push_back(node);
      std::reverse(nodes.begin(), nodes.end());
      return insert_impl_internal_hpf(nodes, update_required);
    }
  }

  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_node_impl_internal_hpt(
      NodeType* node) {
    static_assert(has_parent, "has_parent should be true");

    base::push_down(node);
    const bool is_node_red = is_red(node);

    // Drop node from tree
    NodeType* child = node->left ? node->left : node->right;
    NodeType* parent = node->parent;
    if (parent) ((parent->left == node) ? parent->left : parent->right) = child;
    if (child) child->set_parent(parent);
    if constexpr (reset_links) node->reset_links_and_update_subtree_data();

    // Fix colors
    if (is_node_red)
      return (parent ? subtree_data::propagate_and_find_root(parent) : child);
    while (true) {
      if (is_red(child)) {
        set_black(child);
        return (parent ? subtree_data::propagate_and_find_root(parent) : child);
      }
      if (!parent) return child;
      NodeType* sibling = base::sibling(child, parent);
      assert(sibling);
      sibling->apply_deferred();
      if (is_red(sibling)) {
        assert(is_black(parent));
        base::rotate<false, false>(sibling, parent, parent->parent);
        set_black(sibling);
        set_red(parent);
        sibling = base::sibling(child, parent);
        assert(sibling);
        sibling->apply_deferred();
      }
      assert(is_black(sibling));
      if (is_black(parent) && is_black(sibling->left) &&
          is_black(sibling->right)) {
        set_red(sibling);
        child = parent;
        child->update_subtree_data();
        parent = child->parent;
        continue;
      }
      if (is_red(parent) && is_black(sibling->left) &&
          is_black(sibling->right)) {
        set_red(sibling);
        set_black(parent);
        return subtree_data::propagate_and_find_root(parent);
      }
      if ((parent->left == child) && is_black(sibling->right)) {
        assert(is_red(sibling->left));
        base::rotate<false, true>(sibling->left, sibling, parent);
        set_red(sibling);
        sibling = sibling->parent;
        set_black(sibling);
      } else if ((parent->right == child) && is_black(sibling->left)) {
        assert(is_red(sibling->right));
        base::rotate<false, true>(sibling->right, sibling, parent);
        set_red(sibling);
        sibling = sibling->parent;
        set_black(sibling);
      }
      if (is_red(parent)) {
        set_red(sibling);
        set_black(parent);
      }
      set_black((parent->left == child) ? sibling->right : sibling->left);
      base::rotate<false, false>(sibling, parent, parent->parent);
      return subtree_data::propagate_and_find_root(sibling);
    }
    assert(false);
    return nullptr;
  }

  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_impl(
      NodeType* root, const Key& key, NodeType*& removed_node) {
    removed_node = base::find(root, key);
    return (removed_node
                ? remove_node_impl_internal_hpt<reset_links>(removed_node)
                : root);
  }

  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_at_impl(
      NodeType* root, size_t index, NodeType*& removed_node) {
    removed_node = base::at(root, index);
    return (removed_node
                ? remove_node_impl_internal_hpt<reset_links>(removed_node)
                : root);
  }

  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_node_impl(NodeType* node) {
    return remove_node_impl_internal_hpt<reset_links>(node);
  }

  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_right_impl(
      NodeType* root, NodeType*& removed_node) {
    for (root->apply_deferred(); root->right; root->apply_deferred())
      root = root->right;
    removed_node = root;
    return remove_node_impl_internal_hpt<reset_links>(removed_node);
  }

  /**
   * @brief Implementation of two-way join operation.
   *
   * This function joins two trees while maintaining balance:
   * - Removes the rightmost node from the left tree
   * - Uses that node to join the trees in a balanced way
   * Code is similar to SelfBalancingTree::join_impl
   *
   * @param l The root of the left tree
   * @param r The root of the right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join_impl(NodeType* l, NodeType* r) {
    if (!l) return r;
    if (!r) return l;
    NodeType* node = nullptr;
    l = remove_right_impl<true>(l, node);
    assert(node);
    return join3_impl(l, node, r);
  }

  /**
   * @brief Base implementation of three-way join operation.
   *
   * This function joins three trees in a balanced way:
   * - The middle node becomes the root
   * - Left and right trees become its children
   * - Updates parent links and subtree data
   * - Sets the middle node to red
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @return Pointer to the new root of the joined tree (m1)
   */
  [[nodiscard]] static constexpr NodeType* join3_base_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r) {
    m1->set_left(l);
    m1->set_right(r);
    m1->update_subtree_data();
    set_red(m1);
    return m1;
  }

  [[nodiscard]] static constexpr NodeType* join3_left_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r,
                                                           int bhd) {
    if (is_black(l) && (bhd == 0)) return join3_base_impl(l, m1, r);
    l->apply_deferred();
    l->set_right(join3_left_impl(l->right, m1, r, bhd - (is_black(l) ? 1 : 0)));
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
                                                            int bhd) {
    if (is_black(r) && (bhd == 0)) return join3_base_impl(l, m1, r);
    r->apply_deferred();
    r->set_left(join3_right_impl(l, m1, r->left, bhd - (is_black(r) ? 1 : 0)));
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

  [[nodiscard]] static constexpr NodeType* join3_impl_bhd(NodeType* l,
                                                          NodeType* m1,
                                                          NodeType* r,
                                                          int bhd) {
    return (bhd > 0)   ? join3_left_impl(l, m1, r, bhd)
           : (bhd < 0) ? join3_right_impl(l, m1, r, -bhd)
                       : join3_base_impl(l, m1, r);
  }

  [[nodiscard]] static constexpr NodeType* join3_impl(NodeType* l, NodeType* m1,
                                                      NodeType* r) {
    auto root = join3_impl_bhd(l, m1, r, black_height(l) - black_height(r));
    set_black(root);
    root->set_parent(nullptr);
    return root;
  }

  [[nodiscard]] static constexpr std::pair<NodeType*, int> join3_impl_internal(
      NodeType* l, NodeType* m1, NodeType* r, int bhl, int bhr) {
    auto root = join3_impl_bhd(l, m1, r, bhl - bhr);
    auto bh = std::max(bhl, bhr);
    if (is_red(root)) {
      set_black(root);
      ++bh;
    }
    return {root, bh};
  }

  /**
   * @brief Implementation of split operation by key.
   *
   * This function splits the tree at a given key while maintaining balance:
   * - Recursively splits the tree based on key comparison
   * - Maintains parent links and subtree data
   * - Ensures both parts remain balanced
   *
   * @param root The root of the tree to split
   * @param key The key to split at
   * @param output_l Reference to store the left part (keys < key)
   * @param output_r Reference to store the right part (keys >= key)
   */
  static constexpr void split_impl(NodeType* root, const Key& key,
                                   NodeType*& output_l, NodeType*& output_r) {
    output_l = nullptr;
    output_r = nullptr;
    int bhl = 0, bhr = 0;
    split_impl_internal(root, black_height(root), key, output_l, bhl, output_r,
                        bhr);
    if (output_l) output_l->set_parent(nullptr);
    if (output_r) output_r->set_parent(nullptr);
  }

  static constexpr void split_impl_internal(NodeType* root, int bh,
                                            const Key& key, NodeType*& output_l,
                                            int& bhl, NodeType*& output_r,
                                            int& bhr) {
    root->apply_deferred();
    NodeType *l = root->left, *r = root->right, *m = nullptr;
    root->set_left(nullptr);
    root->set_right(nullptr);
    if (is_black(root)) --bh;
    int bhm = 0;
    if (root->key < key) {
      if (r) split_impl_internal(r, bh, key, m, bhm, output_r, bhr);
      std::tie(output_l, bhl) = join3_impl_internal(l, root, m, bh, bhm);
    } else {
      if (l) split_impl_internal(l, bh, key, output_l, bhl, m, bhm);
      std::tie(output_r, bhr) = join3_impl_internal(m, root, r, bhm, bh);
    }
  }

  /**
   * @brief Implementation of split operation by index.
   *
   * This function splits the tree at a given inorder index while maintaining
   * balance:
   * - Recursively splits the tree based on subtree sizes
   * - Maintains parent links and subtree data
   * - Ensures both parts remain balanced
   *
   * @param root The root of the tree to split
   * @param lsize The size of the left part
   * @param output_l Reference to store the left part
   * @param output_r Reference to store the right part
   */
  static constexpr void split_at_impl(NodeType* root, size_t lsize,
                                      NodeType*& output_l,
                                      NodeType*& output_r) {
    int bhl = 0, bhr = 0;
    split_at_impl_internal(root, black_height(root), lsize, output_l, bhl,
                           output_r, bhr);
    if (output_l) output_l->set_parent(nullptr);
    if (output_r) output_r->set_parent(nullptr);
  }

  static constexpr void split_at_impl_internal(NodeType* root, int bh,
                                               size_t lsize,
                                               NodeType*& output_l, int& bhl,
                                               NodeType*& output_r, int& bhr) {
    root->apply_deferred();
    NodeType *l = root->left, *r = root->right, *m = nullptr;
    root->set_left(nullptr);
    root->set_right(nullptr);
    if (is_black(root)) --bh;
    int bhm = 0;
    const size_t left_size = bst::subtree_data::size(l);
    if (lsize < left_size) {
      split_at_impl_internal(l, bh, lsize, output_l, bhl, m, bhm);
      std::tie(output_r, bhr) = join3_impl_internal(m, root, r, bhm, bh);
    } else if (lsize == left_size) {
      output_l = l;
      bhl = bh;
      std::tie(output_r, bhr) = join3_impl_internal(nullptr, root, r, 0, bh);
    } else if (lsize == left_size + 1) {
      std::tie(output_l, bhl) = join3_impl_internal(l, root, nullptr, bh, 0);
      output_r = r;
      bhr = bh;
    } else {
      split_at_impl_internal(r, bh, lsize - left_size - 1, m, bhm, output_r,
                             bhr);
      std::tie(output_l, bhl) = join3_impl_internal(l, root, m, bhl, bhm);
    }
  }
};
}  // namespace bst
