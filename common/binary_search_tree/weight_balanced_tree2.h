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

#include <algorithm>
#include <tuple>

namespace bst {

/**
 * @brief Variant Weight-Balanced Tree implementation with alternative balancing
 * criterion.
 *
 * This tree implements a variant of the weight-balanced tree algorithm as
 * described in Adams (1993) and analyzed in Hirai & Yamamoto (2011). Unlike the
 * original WBT algorithm, this variant uses a different weight definition and
 * balancing parameters that provide good performance characteristics.
 *
 * Key differences from the original WBT:
 * - Weight definition: weight = subtree size (without +1 offset)
 * - Balancing parameters: Δ = 3, Γ = 2 (the only integer solution in valid
 * range)
 * - Simpler weight calculations and comparisons
 *
 * Key features:
 * - Automatic weight balancing after insertions and removals
 * - Guaranteed O(log n) time complexity for all operations
 * - Efficient join and split operations with balance maintenance
 * - Support for parent pointers, custom data, aggregators, and deferred
 * operations
 *
 * The tree uses subtree size information stored in each node to maintain
 * balance through rotations. The balancing criterion ensures that for any node,
 * the weight of its left and right subtrees satisfy: Δ * weight(left) ≥
 * weight(right) and Δ * weight(right) ≥ weight(left), where Δ = 3.
 *
 * Reference: Hirai, Y., & Yamamoto, K. (2011). Balancing weight-balanced trees.
 * Journal of Functional Programming, 21(3), 287-307.
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
class WeightBalancedTree2
    : public base::SelfBalancingTree<
          NodesManager<base::Node<
              Data,
              base::SubtreeData<templates::PrependIfMissingT<subtree_data::Size,
                                                             AggregatorsTuple>>,
              base::Deferred<DeferredTuple>, has_parent, has_key, Key>>,
          WeightBalancedTree2<has_key, has_parent, Data, AggregatorsTuple,
                              DeferredTuple, Key, NodesManager>> {
 public:
  /**
   * @brief Balancing parameter Δ that determines when rotations are needed.
   *
   * For a node to be balanced, we must have:
   * - Δ * weight(left) ≥ weight(right)
   * - Δ * weight(right) ≥ weight(left)
   *
   * With Δ = 3, this means no subtree can be more than 3 times heavier
   * than its sibling subtree.
   */
  static constexpr unsigned delta = 3;

  /**
   * @brief Balancing parameter Γ that determines single vs double rotation.
   *
   * When a rotation is needed, Γ determines whether to perform a single
   * rotation or a double rotation based on the weight ratios of the
   * grandchildren.
   */
  static constexpr unsigned gamma = 2;

  using SubtreeDataType = base::SubtreeData<
      templates::PrependIfMissingT<subtree_data::Size, AggregatorsTuple>>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, has_parent, has_key, Key>;
  using Self = WeightBalancedTree2<has_key, has_parent, Data, AggregatorsTuple,
                                   DeferredTuple, Key, NodesManager>;
  using SBTree = base::SelfBalancingTree<NodesManager<NodeType>, Self>;
  using Base = typename SBTree::Base;

  friend Base;
  friend SBTree;

 public:
  /**
   * @brief Constructs a Variant Weight-Balanced Tree with the specified maximum
   * number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr WeightBalancedTree2(size_t max_nodes)
      : SBTree(max_nodes) {}

 protected:
  /**
   * @brief Returns the weight of a node using the variant WBT definition.
   *
   * In the variant WBT algorithm, the weight of a node is simply the
   * number of nodes in its subtree (without the +1 offset used in the
   * original WBT). This simplifies weight calculations and comparisons.
   *
   * @param node The node to get the weight of
   * @return The weight of the node, or 0 if the node is nullptr
   */
  [[nodiscard]] static constexpr unsigned weight(
      const NodeType* node) noexcept {
    return bst::subtree_data::size(node);
  }

  /**
   * @brief Fixes the balance of a node using the variant WBT balancing
   * criterion.
   *
   * This function implements the variant WBT balancing algorithm with
   * parameters Δ = 3 and Γ = 2. The balancing criterion ensures that for any
   * node:
   * - 3 * weight(left) ≥ weight(right)
   * - 3 * weight(right) ≥ weight(left)
   *
   * When these conditions are violated, rotations are performed:
   * - Single rotation: when the heavy child's lighter subtree is small enough
   * - Double rotation: when the heavy child's lighter subtree is too large
   *
   * The Γ = 2 parameter determines the threshold for choosing between single
   * and double rotations.
   *
   * @param root The root of the subtree to balance
   * @return The new root of the balanced subtree
   */
  [[nodiscard]] static constexpr NodeType* fix_balance(NodeType* root) {
    if (!root || (weight(root) <= 2)) return root;
    const auto left_weight = weight(root->left),
               right_weight = weight(root->right);
    if (delta * left_weight < right_weight) {
      auto child = root->right;
      child->apply_deferred();
      if (weight(child->left) < gamma * weight(child->right)) {
        base::rotate<true, false, NodeType>(child, root, nullptr);
        return child;
      } else {
        auto grandchild = child->left;
        base::rotate<false, true, NodeType>(grandchild, child, root);
        base::rotate<true, false, NodeType>(grandchild, root, nullptr);
        return grandchild;
      }
    } else if (delta * right_weight < left_weight) {
      auto child = root->left;
      child->apply_deferred();
      if (weight(child->right) < gamma * weight(child->left)) {
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
   * This function implements the variant WBT joining strategy when the left
   * subtree is significantly heavier than the right subtree. It uses the
   * Δ = 3 parameter to determine when to use the base implementation vs.
   * recursive joining.
   *
   * The function checks if weight(left) ≤ Δ * weight(right). If so, it uses
   * the base implementation. Otherwise, it recursively joins in the right
   * subtree of the left tree and then rebalances.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @param awr The adjusted weight threshold for the right tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_left_impl(NodeType* l,
                                                           NodeType* m1,
                                                           NodeType* r,
                                                           unsigned awr) {
    if (weight(l) <= awr) return SBTree::join3_base_impl(l, m1, r);
    l->apply_deferred();
    l->set_right(join3_left_impl(l->right, m1, r, awr));
    l->update_subtree_data();
    return fix_balance(l);
  }

  /**
   * @brief Joins three trees with right subtree potentially heavier.
   *
   * This function implements the variant WBT joining strategy when the right
   * subtree is significantly heavier than the left subtree. It uses the
   * Δ = 3 parameter to determine when to use the base implementation vs.
   * recursive joining.
   *
   * The function checks if weight(right) ≤ Δ * weight(left). If so, it uses
   * the base implementation. Otherwise, it recursively joins in the left
   * subtree of the right tree and then rebalances.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @param awl The adjusted weight threshold for the left tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_right_impl(NodeType* l,
                                                            NodeType* m1,
                                                            NodeType* r,
                                                            unsigned awl) {
    if (weight(r) <= awl) return SBTree::join3_base_impl(l, m1, r);
    r->apply_deferred();
    r->set_left(join3_right_impl(l, m1, r->left, awl));
    r->update_subtree_data();
    return fix_balance(r);
  }

  /**
   * @brief Implementation of three-way join operation for Variant
   * Weight-Balanced Trees.
   *
   * This function implements the variant WBT joining strategy using the Δ = 3
   * parameter to determine the joining approach. It chooses between different
   * joining strategies based on the relative weights of the left and right
   * subtrees.
   *
   * The function uses the variant WBT balance criterion:
   * - If weight(left) > Δ * weight(right): use join3_left_impl
   * - If weight(right) > Δ * weight(left): use join3_right_impl
   * - Otherwise: use the base implementation
   *
   * The adjusted weight thresholds ensure that the resulting tree maintains
   * the variant WBT balance properties. The thresholds are set to Δ * weight
   * to guarantee that the join operation won't break the balance constraints.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @return The root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_impl(NodeType* l, NodeType* m1,
                                                      NodeType* r) {
    const auto wl = weight(l), wr = weight(r);
    return (wl > delta * wr)
               ? join3_left_impl(l, m1, r, std::max(1u, delta * wr))
           : (wr > delta * wl)
               ? join3_right_impl(l, m1, r, std::max(1u, delta * wl))
               : SBTree::join3_base_impl(l, m1, r);
  }
};

}  // namespace bst
