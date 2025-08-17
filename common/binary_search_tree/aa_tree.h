#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/self_balancing_tree.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/balance/aa_level.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <algorithm>
#include <iostream>
#include <tuple>

namespace bst {
template <bool has_key, bool has_parent, typename Data,
          typename AggregatorsTuple = std::tuple<subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t,
          template <class> class NodesManager = memory::ContiguousNodesManager>
class AATree
    : public base::SelfBalancingTree<
          NodesManager<base::Node<
              Data,
              base::SubtreeData<templates::PrependIfMissingT<
                  subtree_data::AALevel, AggregatorsTuple>>,
              base::Deferred<DeferredTuple>, has_parent, has_key, Key>>,
          AATree<has_key, has_parent, Data, AggregatorsTuple, DeferredTuple,
                 Key, NodesManager>> {
 public:
  using SubtreeDataType = base::SubtreeData<
      templates::PrependIfMissingT<subtree_data::AALevel, AggregatorsTuple>>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, has_parent, has_key, Key>;
  using Self = AATree<has_key, has_parent, Data, AggregatorsTuple,
                      DeferredTuple, Key, NodesManager>;
  using SBTree = base::SelfBalancingTree<NodesManager<NodeType>, Self>;
  using Base = typename SBTree::Base;

  friend Base;
  friend SBTree;

  static constexpr bool support_join = false;
  static constexpr bool support_split = false;
  static constexpr bool support_reverse_subtree = false;

 public:
  /**
   * @brief Constructs an AA tree with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr AATree(size_t max_nodes)
      : SBTree(max_nodes) {}

 protected:
  /**
   * @brief Returns the level of a node.
   *
   * @param node The node to get the level of
   * @return The level of the node, or 0 if the node is nullptr
   */
  [[nodiscard]] static constexpr unsigned level(const NodeType* node) noexcept {
    return node ? subtree_data::AALevel::get(node) : 0;
  }

  static constexpr void set_level(NodeType* node, unsigned new_level) noexcept {
    subtree_data::AALevel::set(node, new_level);
  }

  static constexpr void inc_level(NodeType* node) noexcept {
    subtree_data::AALevel::inc(node);
  }

  [[nodiscard]] static constexpr NodeType* aa_skew(NodeType* node) noexcept {
    NodeType* child = node->left;
    assert(level(node) >= level(child));
    if (child && (level(child) == level(node))) {
      // Rotate right
      child->apply_deferred();
      node->set_left(child->right);
      child->set_right(node);
      node->update_subtree_data();
      child->set_parent(nullptr);
      child->update_subtree_data();
      return child;
    }
    return node;
  }

  [[nodiscard]] static constexpr NodeType* aa_split(NodeType* node) noexcept {
    NodeType* child = node->right;
    assert(level(node) >= level(child));
    if (child && (level(child->right) == level(node))) {
      // Rotate left
      child->apply_deferred();
      node->set_right(child->left);
      child->set_left(node);
      node->update_subtree_data();
      child->set_parent(nullptr);
      child->update_subtree_data();
      inc_level(child);
      return child;
    }
    return node;
  }

  [[nodiscard]] static constexpr NodeType* fix_balance_insert(NodeType* root) {
    root = aa_skew(root);
    root = aa_split(root);
    return root;
  }

  [[nodiscard]] static constexpr NodeType* fix_balance_remove(NodeType* root) {
    if (!root) return nullptr;
    const auto new_level = std::min(level(root->left), level(root->right)) + 1;
    if (level(root) > new_level) {
      assert(new_level == level(root) - 1);
      set_level(root, new_level);
      if (level(root->right) > new_level) set_level(root->right, new_level);
      root = aa_skew(root);
      if (root->right) {
        root->right->apply_deferred();
        root->set_right(aa_skew(root->right));
        NodeType* child = root->right;
        if (child->right) {
          child->right->apply_deferred();
          child->set_right(aa_skew(child->right));
          child->update_subtree_data();  // Only if something changed.
        }
      }
      root = aa_split(root);
      if (root->right) {
        root->right->apply_deferred();
        root->set_right(aa_split(root->right));
      }
      root->update_subtree_data();
    }
    return root;
  }

  /**
   * @brief Base implementation of three-way join.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_base_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r) {
    assert(level(l) == level(r));
    SBTree::join3_base_impl(l, m1, r);
    subtree_data::AALevel::set(m1, level(l) + 1);
    return m1;
  }

  /**
   * @brief Joins three trees with left subtree potentially taller.
   *
   * This function is used when the left subtree is significantly taller
   * than the right subtree. It ensures that the resulting tree maintains
   * the WAVL balance property.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @param lr The level of the right tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_left_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r,
                                                           unsigned lr) {
    assert(level(l) >= lr);
    if (level(l) == lr) return join3_base_impl(l, m1, r);
    l->apply_deferred();
    l->set_right(join3_left_impl(l->right, m1, r, lr));
    l->update_subtree_data();
    return fix_balance_insert(l);
  }

  /**
   * @brief Joins three trees with right subtree potentially taller.
   *
   * This function is used when the right subtree is significantly taller
   * than the left subtree. It ensures that the resulting tree maintains
   * the WAVL balance property.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @param ll The level of the left tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_right_impl(NodeType* l,
                                                            NodeType* m1,
                                                            NodeType* r,
                                                            unsigned ll) {
    assert(level(r) >= ll);
    if (level(r) == ll) return join3_base_impl(l, m1, r);
    r->apply_deferred();
    r->set_left(join3_right_impl(l, m1, r->left, ll));
    r->update_subtree_data();
    return fix_balance_insert(r);
  }

  /**
   * @brief Implementation of three-way join operation for WAVL trees.
   *
   * This function joins three trees while maintaining the WAVL balance
   * property. It chooses between different joining strategies based on
   * the relative ranks of the subtrees.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_impl(NodeType* l, NodeType* m1,
                                                      NodeType* r) {
    const auto ll = level(l), lr = level(r);
    return (ll > lr)   ? join3_left_impl(l, m1, r, lr)
           : (ll < lr) ? join3_right_impl(l, m1, r, ll)
                       : join3_base_impl(l, m1, r);
  }
};

}  // namespace bst
