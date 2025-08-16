#pragma once

#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/wavl_tree.h"
#include "common/memory/contiguous_nodes_manager.h"

#include <tuple>

/**
 * @brief A default binary search tree implementation using WAVL tree.
 *
 * This is an alias for bst::WAVLTree with sensible defaults:
 * - has_key = true (supports key-based operations)
 * - has_parent = true (maintains parent pointers for efficient operations)
 * - Data = int64_t (default data type)
 * - AggregatorsTuple = std::tuple<subtree_data::Size> (includes size tracking)
 * - DeferredTuple = std::tuple<> (no deferred operations by default)
 * - Key = int64_t (default key type)
 * - NodesManager = memory::ContiguousNodesManager (default memory manager)
 *
 * WAVL tree provides guaranteed O(log n) time complexity for all operations
 * while maintaining good balance through rank-based balancing.
 *
 * Template parameter order matches WAVLTree exactly:
 * @tparam has_key Whether the tree uses keys for ordering (default: true)
 * @tparam has_parent Whether nodes maintain parent pointers (default: true)
 * @tparam Data The data type stored in each node (default: int64_t)
 * @tparam AggregatorsTuple Tuple of aggregator types for subtree data (default:
 * std::tuple<bst::subtree_data::Size>)
 * @tparam DeferredTuple Tuple of deferred operation types (default:
 * std::tuple<>)
 * @tparam Key The key type used for ordering (default: int64_t)
 * @tparam NodesManager The node manager type for memory management (default:
 * memory::ContiguousNodesManager)
 */
template <bool has_key = true, bool has_parent = true, typename Data = int64_t,
          typename AggregatorsTuple = std::tuple<bst::subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t,
          template <class> class NodesManager = memory::ContiguousNodesManager>
using BinarySearchTree =
    bst::WAVLTree<has_key, has_parent, Data, AggregatorsTuple, DeferredTuple,
                  Key, NodesManager>;
