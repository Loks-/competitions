#pragma once

#include "common/binary_search_tree/base/base_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"

#include <vector>

namespace bst {

/**
 * @brief A static binary search tree implementation that supports only
 * build-time operations.
 *
 * This tree is designed for scenarios where the tree structure is built once
 * and remains unchanged throughout its lifetime. It supports operations that
 * don't modify the tree structure, such as:
 * - Finding nodes by key
 * - Applying deferred computations
 * - Querying subtree data
 * - Traversing the tree
 *
 * The tree is built using a balanced construction algorithm that ensures
 * optimal height and maintains the binary search tree property.
 *
 * @tparam has_parent Whether nodes maintain parent pointers
 * @tparam Data The data type stored in each node
 * @tparam AggregatorsTuple Tuple of aggregator types for subtree data
 * @tparam DeferredTuple Tuple of deferred operation types
 * @tparam Key The key type used for ordering
 */
template <bool has_parent, class Data,
          class AggregatorsTuple = std::tuple<subtree_data::Size>,
          class DeferredTuple = std::tuple<>, class Key = int64_t>
class StaticTree
    : public base::BaseTree<
          memory::ContiguousNodesManager<
              base::Node<Data, base::SubtreeData<AggregatorsTuple>,
                         base::Deferred<DeferredTuple>, has_parent, true, Key>>,
          StaticTree<has_parent, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  using SubtreeDataType = base::SubtreeData<AggregatorsTuple>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, has_parent, true, Key>;
  using Self =
      StaticTree<has_parent, Data, AggregatorsTuple, DeferredTuple, Key>;
  using Base = base::BaseTree<memory::ContiguousNodesManager<NodeType>, Self>;

  /**
   * @brief Operation support flags.
   *
   * This tree only supports operations that don't modify its structure.
   * All modification operations are disabled.
   */
  static constexpr bool support_insert = false;
  static constexpr bool support_remove = false;
  static constexpr bool support_join = false;
  static constexpr bool support_split = false;

  /**
   * @brief Constructs a static tree with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr StaticTree(size_t max_nodes)
      : Base(max_nodes) {}

  static constexpr NodeType* insert(NodeType* root, NodeType* node) = delete;
  static constexpr NodeType* insert_at(NodeType* root, NodeType* node,
                                       size_t index) = delete;
  constexpr NodeType* insert_new(NodeType* root, const Data& data,
                                 const Key& key) = delete;
  constexpr NodeType* insert_new_at(NodeType* root, const Data& data,
                                    size_t index) = delete;
  static constexpr NodeType* remove(NodeType* root, const Key& key,
                                    NodeType*& removed_node) = delete;
  static constexpr NodeType* remove_at(NodeType* root, size_t index,
                                       NodeType*& removed_node) = delete;
  static constexpr NodeType* remove_node(NodeType* node) = delete;
  constexpr NodeType* remove_and_release(NodeType* root,
                                         const Key& key) = delete;
  constexpr NodeType* remove_and_release_at(NodeType* root,
                                            size_t index) = delete;
  constexpr NodeType* remove_and_release_node(NodeType* node) = delete;
  static constexpr NodeType* join(NodeType* l, NodeType* r) = delete;
  static constexpr NodeType* join3(NodeType* l, NodeType* m1,
                                   NodeType* r) = delete;
};

}  // namespace bst
