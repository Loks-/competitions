#pragma once

#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/self_balancing_tree.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"

namespace bst {

/**
 * @brief A basic binary search tree implementation without automatic balancing.
 *
 * This tree implements all standard BST operations but doesn't perform any
 * automatic balancing. It's useful in scenarios where:
 * - The tree is small and balancing overhead isn't needed
 * - Exact tree structure needs to be controlled
 * - Performance of basic operations is more important than worst-case
 * guarantees
 *
 * The tree maintains the binary search tree property but doesn't guarantee
 * logarithmic height. Operations like insert, remove, join, and split are
 * implemented without any balancing logic.
 *
 * @tparam has_key Whether the tree uses keys for ordering
 * @tparam has_parent Whether nodes maintain parent pointers
 * @tparam Data The data type stored in each node
 * @tparam AggregatorsTuple Tuple of aggregator types for subtree data
 * @tparam DeferredTuple Tuple of deferred operation types
 * @tparam Key The key type used for ordering (if has_key is true)
 */
template <bool has_key, bool has_parent, typename Data,
          typename AggregatorsTuple = std::tuple<subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t>
class UnbalancedTree
    : public base::SelfBalancingTree<
          memory::ContiguousNodesManager<base::Node<
              Data, base::SubtreeData<AggregatorsTuple>,
              base::Deferred<DeferredTuple>, has_parent, has_key, Key>>,
          UnbalancedTree<has_key, has_parent, Data, AggregatorsTuple,
                         DeferredTuple, Key>> {
 public:
  using SubtreeDataType = base::SubtreeData<AggregatorsTuple>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, has_parent, has_key, Key>;
  using Self = UnbalancedTree<has_key, has_parent, Data, AggregatorsTuple,
                              DeferredTuple, Key>;
  using SBTree =
      base::SelfBalancingTree<memory::ContiguousNodesManager<NodeType>, Self>;
  using Base = typename SBTree::Base;

  friend Base;
  friend SBTree;

 public:
  /**
   * @brief Constructs an unbalanced tree with the specified maximum number of
   * nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr UnbalancedTree(size_t max_nodes)
      : SBTree(max_nodes) {}

 protected:
  /**
   * @brief Fixes the balance of a node after an operation.
   *
   * This implementation does nothing as the tree doesn't perform any
   * balancing. It simply returns the node unchanged.
   *
   * @param node The node that needs balance fixing
   * @return The node unchanged
   */
  [[nodiscard]] static constexpr NodeType* fix_balance(NodeType* node) {
    return node;
  }

  /**
   * @brief Implementation of three-way join operation.
   *
   * This implementation simply connects the nodes without any balancing.
   * The middle node becomes the root, and left and right trees become
   * its children.
   *
   * @param l The root of the left tree
   * @param m1 The middle node
   * @param r The root of the right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3_impl(NodeType* l, NodeType* m1,
                                                      NodeType* r) {
    return SBTree::join3_base_impl(l, m1, r);
  }
};

}  // namespace bst
