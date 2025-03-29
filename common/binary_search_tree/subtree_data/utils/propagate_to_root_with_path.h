#pragma once

#include <vector>

namespace bst {
namespace subtree_data {

/**
 * @brief Updates subtree data from a node up to the root using a provided path.
 *
 * This function ensures that all subtree data aggregators are updated along the
 * path from the given node to the root. It's used when node's data, keys,
 * structure, or node order has changed and we need to propagate these changes
 * upward.
 *
 * Unlike propagate_to_root, this function uses a pre-computed path to the root
 * instead of parent links. This is useful when parent links are not available
 * or when we already have the path computed for other purposes.
 *
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void propagate_to_root_with_path(const std::vector<Node*>& path,
                                        size_t start_from_index = 0) {
  if constexpr (!Node::SubtreeDataType::empty) {
    // Update subtree data from node up to root
    for (auto it = path.begin() + start_from_index; it != path.end(); ++it) {
      (*it)->update_subtree_data();
    }
  }
}

/**
 * @brief Updates only data-related subtree aggregators using a provided path.
 *
 * Used when only the node's data has changed, not its structure, keys, or
 * node order. This is typically used for simple data updates that don't
 * affect the tree's structure or ordering.
 *
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void propagate_for_data_update_with_path(const std::vector<Node*>& path,
                                                size_t start_from_index = 0) {
  if constexpr (Node::SubtreeDataType::use_data) {
    propagate_to_root_with_path<Node>(path, start_from_index);
  }
}

/**
 * @brief Updates only key-related subtree aggregators using a provided path.
 *
 * Used when only the node's keys have changed, not its data, structure, or
 * node order. This is typically used for key updates that don't affect
 * the tree's structure or ordering.
 *
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void propagate_for_key_update_with_path(const std::vector<Node*>& path,
                                               size_t start_from_index = 0) {
  if constexpr (Node::SubtreeDataType::use_keys) {
    propagate_to_root_with_path<Node>(path, start_from_index);
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
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void propagate_for_structure_update_with_path(
    const std::vector<Node*>& path, size_t start_from_index = 0) {
  if constexpr (Node::SubtreeDataType::use_tree_structure) {
    propagate_to_root_with_path<Node>(path, start_from_index);
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
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void propagate_for_nodes_order_update_with_path(
    const std::vector<Node*>& path, size_t start_from_index = 0) {
  if constexpr (Node::SubtreeDataType::use_nodes_order) {
    propagate_to_root_with_path<Node>(path, start_from_index);
  }
}

}  // namespace subtree_data
}  // namespace bst
