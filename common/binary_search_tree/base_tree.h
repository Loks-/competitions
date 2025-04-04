#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/self_balancing_tree.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"

namespace bst {
/**
 * @brief Simple unbalanced binary search tree implementation.
 *
 * This class provides a basic binary search tree implementation without
 * any balancing. It inherits from BalancedTree but overrides the balancing
 * functions to do nothing, effectively creating an unbalanced tree.
 *
 * Key features:
 * - Simple insertion and removal without rebalancing
 * - Inherits all basic BST operations from BalancedTree
 * - Useful for testing and small trees where balance isn't critical
 * - Supports parent pointers, custom data, aggregators, and deferred operations
 *
 * @tparam use_parent Whether to maintain parent pointers
 * @tparam TData Type of data stored in nodes
 * @tparam TAggregatorsTuple Tuple of aggregator types for subtree data
 * @tparam TDeferredTuple Tuple of deferred operation types
 * @tparam TKey Type of keys used in the tree
 */
template <bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t>
class BaseTree
    : public base::SelfBalancingTree<
          memory::ContiguousNodesManager<base::Node<
              TData, base::SubtreeData<TAggregatorsTuple>,
              base::Deferred<TDeferredTuple>, use_parent, true, TKey>>,
          BaseTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple,
                   TKey>> {
 public:
  using Self =
      BaseTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple, TKey>;
  using NodeType =
      base::Node<TData, base::SubtreeData<TAggregatorsTuple>,
                 base::Deferred<TDeferredTuple>, use_parent, true, TKey>;
  using TNode = NodeType;  // temporary
  using NodesManagerType = memory::ContiguousNodesManager<NodeType>;
  using Base = base::SelfBalancingTree<NodesManagerType, Self>;

  friend Base;

  static constexpr bool support_join3 = true;

 public:
  /**
   * @brief Constructs a base tree with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  explicit BaseTree(size_t max_nodes) : Base(max_nodes) {}

 protected:
  /**
   * @brief No-op balance fixing since this is an unbalanced tree.
   */
  static NodeType* fix_balance(NodeType* node) { return node; }

 public:
  /**
   * @brief Joins three trees together.
   *
   * @param l Left tree
   * @param m1 Middle node
   * @param r Right tree
   * @return Root of the resulting tree
   */
  static NodeType* join3_impl(NodeType* l, NodeType* m1, NodeType* r) {
    return Base::join3_base_impl(l, m1, r);
  }
};
}  // namespace bst
