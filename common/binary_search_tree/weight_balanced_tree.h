#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/self_balancing_tree.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <tuple>

namespace bst {

/**
 * @brief Weight-Balanced Tree implementation with automatic weight balancing.
 *
 * This tree implements a self-balancing binary search tree where the weights
 * of the two child subtrees of any node are kept in balance. The weight of
 * a subtree is the number of nodes it contains, and the tree maintains the
 * property that no subtree is significantly larger than its sibling.
 *
 * Key features:
 * - Automatic weight balancing after insertions and removals
 * - Guaranteed O(log n) time complexity for all operations
 * - Efficient join and split operations with balance maintenance
 * - Support for parent pointers, custom data, aggregators, and deferred
 * operations
 *
 * The tree uses subtree size information stored in each node to maintain
 * balance through rotations. It automatically adds size tracking to the subtree
 * data if not already present. The balancing criterion ensures that the weight
 * of any subtree is at most a constant factor times the weight of its sibling.
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
class WeightBalancedTree
    : public base::SelfBalancingTree<
          NodesManager<base::Node<
              Data,
              base::SubtreeData<templates::PrependIfMissingT<subtree_data::Size,
                                                             AggregatorsTuple>>,
              base::Deferred<DeferredTuple>, has_parent, has_key, Key>>,
          WeightBalancedTree<has_key, has_parent, Data, AggregatorsTuple,
                             DeferredTuple, Key, NodesManager>> {
 public:
  using SubtreeDataType = base::SubtreeData<
      templates::PrependIfMissingT<subtree_data::Size, AggregatorsTuple>>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, has_parent, has_key, Key>;
  using Self = WeightBalancedTree<has_key, has_parent, Data, AggregatorsTuple,
                                  DeferredTuple, Key, NodesManager>;
  using SBTree = base::SelfBalancingTree<NodesManager<NodeType>, Self>;
  using Base = typename SBTree::Base;

  friend Base;
  friend SBTree;

 public:
  /**
   * @brief Constructs a Weight-Balanced Tree with the specified maximum number
   * of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr WeightBalancedTree(size_t max_nodes)
      : SBTree(max_nodes) {}

 protected:
  /**
   * @brief Returns the weight of a node.
   *
   * The weight of a node is defined as the number of nodes in its subtree
   * plus 1 (the node itself). This is used to determine balance and guide
   * rebalancing operations.
   *
   * @param node The node to get the weight of
   * @return The weight of the node, or 0 if the node is nullptr
   */
  [[nodiscard]] static constexpr uint64_t weight(
      const NodeType* node) noexcept {
    return bst::subtree_data::size(node) + 1;
  }

  /**
   * @brief Fixes the balance of a node using rotations.
   *
   * This function performs the necessary rotations to restore the weight
   * balance property. It handles both single and double rotations depending
   * on the weight ratios of the node and its children.
   *
   * The balancing criterion ensures that for any node, the weight of its
   * left and right subtrees are kept in proportion. If one subtree becomes
   * too heavy (more than half the total weight), rotations are performed
   * to redistribute the weight more evenly.
   *
   * @param root The root of the subtree to balance
   * @return The new root of the balanced subtree
   */
  [[nodiscard]] static constexpr NodeType* fix_balance(NodeType* root) {
    if (!root) return nullptr;
    const uint64_t w = weight(root), sqr_weight = w * w,
                   left_weight = weight(root->left),
                   right_weight = weight(root->right);
    if (2 * right_weight * right_weight > sqr_weight) {
      auto child = root->right;
      child->apply_deferred();
      const uint64_t child_left_weight = weight(child->left),
                     child_right_weight = weight(child->right);
      if (child_left_weight * child_left_weight <
          2 * child_right_weight * child_right_weight) {
        base::rotate<true, false, NodeType>(child, root, nullptr);
        return child;
      } else {
        auto grandchild = child->left;
        base::rotate<false, true, NodeType>(grandchild, child, root);
        base::rotate<true, false, NodeType>(grandchild, root, nullptr);
        return grandchild;
      }
    } else if (2 * left_weight * left_weight > sqr_weight) {
      auto child = root->left;
      child->apply_deferred();
      const uint64_t child_left_weight = weight(child->left),
                     child_right_weight = weight(child->right);
      if (child_right_weight * child_right_weight <
          2 * child_left_weight * child_left_weight) {
        base::rotate<true, false, NodeType>(child, root, nullptr);
        return child;
      } else {
        auto grandchild = child->right;
        base::rotate<false, true, NodeType>(grandchild, child, root);
        base::rotate<true, false, NodeType>(grandchild, root, nullptr);
        return grandchild;
      }
    }
    return root;
  }

  /**
   * @brief Joins three trees with left subtree potentially heavier.
   *
   * This function is used when the left subtree is significantly heavier
   * than the right subtree. It ensures that the resulting tree maintains
   * the weight balance property by recursively joining in the right subtree
   * of the left tree.
   *
   * The function checks if the weight ratio between left and right subtrees
   * is acceptable (within the balancing bounds). If not, it recursively
   * joins in the right subtree of the left tree and then rebalances.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @param wr The weight of the right tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_left_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r,
                                                           uint64_t wr) {
    const uint64_t wl = weight(l);
    if (2 * wl * wl <= (wl + wr) * (wl + wr))
      return SBTree::join3_base_impl(l, m1, r);
    l->apply_deferred();
    l->set_right(join3_left_impl(l->right, m1, r, wr));
    l->update_subtree_data();
    return fix_balance(l);
  }

  /**
   * @brief Joins three trees with right subtree potentially heavier.
   *
   * This function is used when the right subtree is significantly heavier
   * than the left subtree. It ensures that the resulting tree maintains
   * the weight balance property by recursively joining in the left subtree
   * of the right tree.
   *
   * The function checks if the weight ratio between left and right subtrees
   * is acceptable (within the balancing bounds). If not, it recursively
   * joins in the left subtree of the right tree and then rebalances.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @param wl The weight of the left tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_right_impl(NodeType* l,
                                                            NodeType* m1,
                                                            NodeType* r,
                                                            uint64_t wl) {
    const uint64_t wr = weight(r);
    if (2 * wr * wr <= (wl + wr) * (wl + wr))
      return SBTree::join3_base_impl(l, m1, r);
    r->apply_deferred();
    r->set_left(join3_right_impl(l, m1, r->left, wl));
    r->update_subtree_data();
    return fix_balance(r);
  }

  /**
   * @brief Implementation of three-way join operation for Weight-Balanced
   * Trees.
   *
   * This function joins three trees while maintaining the weight balance
   * property. It chooses between different joining strategies based on
   * the relative weights of the left and right subtrees.
   *
   * The function handles three cases:
   * - Left subtree heavier: uses join3_left_impl
   * - Right subtree heavier: uses join3_right_impl
   * - Equal weights: uses the base implementation
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_impl(NodeType* l, NodeType* m1,
                                                      NodeType* r) {
    const auto wl = weight(l), wr = weight(r);
    return (wl > wr) ? join3_left_impl(l, m1, r, wr)
                     : join3_right_impl(l, m1, r, wl);
  }
};

}  // namespace bst
