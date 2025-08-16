#pragma once

#include <vector>

namespace bst {
namespace subtree_data {

/**
 * @brief Updates only data-related subtree aggregators using a provided path.
 *
 * Used when only the node's data has changed, not its structure, keys, or
 * node order. This is typically used for simple data updates that don't
 * affect the tree's structure or ordering.
 *
 * @tparam TNode The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename TNode>
constexpr void propagate_for_data_update_with_path(
    const std::vector<TNode*>& path, size_t start_from_index = 0) {
  if constexpr (TNode::SubtreeDataType::use_data) {
    // Update subtree data from node up to root
    for (auto it = path.begin() + start_from_index; it != path.end(); ++it)
      (*it)->update_subtree_data();
  }
}

/**
 * @brief Updates only key-related subtree aggregators using a provided path.
 *
 * Used when only the node's keys have changed, not its data, structure, or
 * node order. This is typically used for key updates that don't affect
 * the tree's structure or ordering.
 *
 * @tparam TNode The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename TNode>
constexpr void propagate_for_key_update_with_path(
    const std::vector<TNode*>& path, size_t start_from_index = 0) {
  if constexpr (TNode::SubtreeDataType::use_keys) {
    // Update subtree data from node up to root
    for (auto it = path.begin() + start_from_index; it != path.end(); ++it)
      (*it)->update_subtree_data();
  }
}

/**
 * @brief Updates only tree structure-related subtree aggregators using a
 * provided path.
 *
 * Used when only the tree structure has changed (e.g., rotations, rebalancing),
 * not its data, keys, or node order. This is typically used for structural
 * changes that don't affect the data or ordering of nodes.
 *
 * @tparam TNode The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename TNode>
constexpr void propagate_for_structure_update_with_path(
    const std::vector<TNode*>& path, size_t start_from_index = 0) {
  if constexpr (TNode::SubtreeDataType::use_tree_structure) {
    // Update subtree data from node up to root
    for (auto it = path.begin() + start_from_index; it != path.end(); ++it)
      (*it)->update_subtree_data();
  }
}

/**
 * @brief Updates only nodes order-related subtree aggregators using a provided
 * path.
 *
 * Used when only the order of nodes in the subtree has changed, not its
 * data, structure, or keys. This is typically used for operations that
 * reorder nodes but don't change their data or the tree's structure.
 *
 * @tparam TNode The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename TNode>
constexpr void propagate_for_nodes_order_update_with_path(
    const std::vector<TNode*>& path, size_t start_from_index = 0) {
  if constexpr (TNode::SubtreeDataType::use_nodes_order) {
    // Update subtree data from node up to root
    for (auto it = path.begin() + start_from_index; it != path.end(); ++it)
      (*it)->update_subtree_data();
  }
}

}  // namespace subtree_data
}  // namespace bst
