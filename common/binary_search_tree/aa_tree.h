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
#include <tuple>

namespace bst {

/**
 * @brief AA tree implementation with level-based balancing.
 *
 * An AA tree (Arne Andersson tree) is a self-balancing binary search tree
 * that maintains balance using a level system. Each node has a level, and
 * the tree maintains the following properties:
 * - Left children have level strictly less than their parent
 * - Right children have level less than or equal to their parent
 * - Right grandchildren have level strictly less than their grandparent
 * - All leaf nodes are at level 1
 *
 * Key features:
 * - Automatic level balancing after insertions and removals
 * - Guaranteed O(log n) time complexity for all operations
 * - Efficient join and split operations with balance maintenance
 * - Support for parent pointers, custom data, aggregators, and deferred
 * operations
 *
 * The tree uses level information stored in each node to maintain balance
 * through skew and split operations. It automatically adds level tracking
 * to the subtree data if not already present.
 *
 * @tparam has_key Whether the tree uses keys for ordering
 * @tparam has_parent Whether nodes maintain parent pointers
 * @tparam Data The data type stored in each node
 * @tparam AggregatorsTuple Tuple of aggregator types for subtree data
 * @tparam DeferredTuple Tuple of deferred operation types
 * @tparam Key The key type used for ordering (if has_key is true)
 * @tparam NodesManager The node manager type for memory management
 */
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
   * The level is a fundamental property of AA trees that determines
   * the balance structure. Leaf nodes have level 1, and internal nodes
   * have levels that satisfy the AA tree balance properties.
   *
   * @param node The node to get the level of
   * @return The level of the node, or 0 if the node is nullptr
   */
  [[nodiscard]] static constexpr unsigned level(const NodeType* node) noexcept {
    return node ? subtree_data::AALevel::get(node) : 0;
  }

  /**
   * @brief Sets the level of a node.
   *
   * @param node The node to set the level of
   * @param new_level The new level value
   */
  static constexpr void set_level(NodeType* node, unsigned new_level) noexcept {
    subtree_data::AALevel::set(node, new_level);
  }

  /**
   * @brief Increments the level of a node.
   *
   * This is commonly used during balancing operations when a node
   * needs to be promoted to a higher level.
   *
   * @param node The node to increment the level of
   */
  static constexpr void inc_level(NodeType* node) noexcept {
    subtree_data::AALevel::inc(node);
  }

  /**
   * @brief Performs a right rotation around the parent-child pair.
   *
   * A right rotation moves the left child up to become the new parent,
   * while the original parent becomes the right child of the new parent.
   * This operation is used in the skew operation to maintain AA tree
   * balance properties.
   *
   * @tparam update_child Whether to update the child's subtree data
   * @param parent The original parent node
   * @param child The left child node to rotate up
   * @return The new root of the rotated subtree
   */
  template <bool update_child>
  static constexpr NodeType* rotate_right(NodeType* parent, NodeType* child) {
    child->apply_deferred();
    parent->set_left(child->right);
    child->set_right(parent);
    parent->update_subtree_data();
    if constexpr (update_child) child->update_subtree_data();
    return child;
  }

  /**
   * @brief Performs a left rotation around the parent-child pair.
   *
   * A left rotation moves the right child up to become the new parent,
   * while the original parent becomes the left child of the new parent.
   * This operation is used in the split operation and also increments
   * the level of the promoted child.
   *
   * @tparam update_child Whether to update the child's subtree data
   * @param parent The original parent node
   * @param child The right child node to rotate up
   * @return The new root of the rotated subtree
   */
  template <bool update_child>
  static constexpr NodeType* rotate_left(NodeType* parent, NodeType* child) {
    child->apply_deferred();
    parent->set_right(child->left);
    child->set_left(parent);
    parent->update_subtree_data();
    inc_level(child);
    if constexpr (update_child) child->update_subtree_data();
    return child;
  }

  /**
   * @brief Performs an AA tree skew operation.
   *
   * The skew operation ensures that left children have levels strictly
   * less than their parent. If a left child has the same level as its
   * parent, a right rotation is performed to fix this violation.
   *
   * @tparam update_child Whether to update child nodes during rotation
   * @param node The node to perform skew on
   * @return The new root of the subtree after skewing
   */
  template <bool update_child>
  [[nodiscard]] static constexpr NodeType* aa_skew(NodeType* node) noexcept {
    NodeType* child = node->left;
    assert(level(node) >= level(child));
    return (child && (level(child) == level(node)))
               ? rotate_right<update_child>(node, child)
               : node;
  }

  /**
   * @brief Performs an AA tree split operation.
   *
   * The split operation ensures that right grandchildren have levels
   * strictly less than their grandparent. If a right grandchild has
   * the same level as its grandparent, a left rotation is performed
   * to fix this violation.
   *
   * @tparam update_child Whether to update child nodes during rotation
   * @param node The node to perform split on
   * @return The new root of the subtree after splitting
   */
  template <bool update_child>
  [[nodiscard]] static constexpr NodeType* aa_split(NodeType* node) noexcept {
    NodeType* child = node->right;
    assert(level(node) >= level(child));
    return (child && (level(child->right) == level(node)))
               ? rotate_left<update_child>(node, child)
               : node;
  }

  /**
   * @brief Builds a balanced AA tree from a sorted array of nodes.
   *
   * This function constructs a perfectly balanced AA tree by recursively
   * selecting the middle element as the root and building left and right
   * subtrees. The level of each node is set based on the levels of its
   * children to maintain AA tree balance properties.
   *
   * @tparam update_leafs Whether to update leaf node subtree data
   * @param nodes Vector of node pointers in sorted order
   * @param begin Starting index in the nodes array
   * @param end Ending index in the nodes array (exclusive)
   * @return Root of the constructed subtree
   */
  template <bool update_leafs>
  [[nodiscard]] static constexpr NodeType* build_tree_base_impl(
      const std::vector<NodeType*>& nodes, size_t begin, size_t end) {
    if (begin >= end) return nullptr;
    const size_t m = (begin + end - 1) / 2;
    NodeType* root = nodes[m];
    root->set_left(build_tree_base_impl<update_leafs>(nodes, begin, m));
    root->set_right(build_tree_base_impl<update_leafs>(nodes, m + 1, end));
    [[maybe_unused]] const auto left_level = level(root->left),
                                right_level = level(root->right);
    assert((left_level <= right_level) && (right_level <= left_level + 1));
    set_level(root, left_level + 1);
    if (update_leafs || (end - begin > 1)) root->update_subtree_data();
    return root;
  }

  /**
   * @brief Fixes balance after insertion operations.
   *
   * This function restores AA tree balance properties after a node
   * has been inserted. It handles cases where:
   * - Both children have the same level as the parent (promote parent)
   * - Left child has same level as parent (right rotation)
   * - Right grandchild has same level as grandparent (left rotation)
   *
   * @param root The root of the subtree to rebalance
   * @return The new root of the rebalanced subtree
   */
  [[nodiscard]] static constexpr NodeType* fix_balance_insert(NodeType* root) {
    NodeType *left = root->left, *right = root->right;
    const auto root_level = level(root);
    assert(root_level >= level(left) && root_level >= level(right));
    if (level(left) == root_level) {
      if (level(right) == root_level) {
        inc_level(root);
      } else {
        root = rotate_right<true>(root, left);
        root->set_parent(nullptr);
      }
    } else if (right && (root_level == level(right->right))) {
      root = rotate_left<true>(root, right);
      root->set_parent(nullptr);
    }
    return root;
  }

  /**
   * @brief Fixes balance after removal operations.
   *
   * This function restores AA tree balance properties after a node
   * has been removed. It performs a series of skew and split operations
   * to ensure that all AA tree balance properties are maintained.
   *
   * The function:
   * - Adjusts the level of the current node if necessary
   * - Applies skew operations to fix left child level violations
   * - Applies split operations to fix right grandchild level violations
   * - Recursively fixes balance in the right subtree
   *
   * @param root The root of the subtree to rebalance
   * @return The new root of the rebalanced subtree, or nullptr if empty
   */
  [[nodiscard]] static constexpr NodeType* fix_balance_remove(NodeType* root) {
    if (!root) return nullptr;
    const auto new_level = std::min(level(root->left), level(root->right)) + 1;
    if (level(root) > new_level) {
      assert(new_level == level(root) - 1);
      set_level(root, new_level);
      if (level(root->right) > new_level) set_level(root->right, new_level);
      root = aa_skew<false>(root);
      if (root->right) {
        root->right->apply_deferred();
        root->set_right(aa_skew<true>(root->right));
        NodeType* child = root->right;
        NodeType* grandchild = child->right;
        if (grandchild) {
          grandchild->apply_deferred();
          child->set_right(aa_skew<true>(grandchild));
          if constexpr (SubtreeDataType::use_tree_structure) {
            if (child->right != grandchild) child->update_subtree_data();
          }
        }
      }
      root = aa_split<false>(root);
      if (root->right) {
        root->right->apply_deferred();
        root->set_right(aa_split<true>(root->right));
      }
      root->set_parent(nullptr);
      root->update_subtree_data();
    }
    return root;
  }

  /**
   * @brief Base implementation of three-way join.
   *
   * This function joins three trees when the left and right subtrees
   * have equal levels. The middle node becomes the new root with a
   * level one higher than the subtrees.
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
   * the AA tree balance properties by recursively joining in the right
   * subtree of the left tree.
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
   * the AA tree balance properties by recursively joining in the left
   * subtree of the right tree.
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
   * @brief Implementation of three-way join operation for AA trees.
   *
   * This function joins three trees while maintaining the AA tree balance
   * properties. It chooses between different joining strategies based on
   * the relative levels of the left and right subtrees.
   *
   * The function handles three cases:
   * - Left subtree taller: uses join3_left_impl
   * - Right subtree taller: uses join3_right_impl
   * - Equal heights: uses join3_base_impl
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
