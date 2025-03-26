#pragma once

#include <vector>

namespace bst {
namespace subtree_data {

/**
 * @brief Updates subtree data from a node up to the root using a provided path.
 *
 * This function ensures that all subtree data aggregators are updated along the
 * path from the given node to the root. It's used when node's data or its
 * subtree structure has changed and we need to propagate these changes upward.
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
template <class Node>
inline void propagate_to_root_with_path(const std::vector<Node*>& path,
                                        size_t start_from_index = 0) {
  if constexpr (!Node::TInfo::empty) {
    // Update subtree data from node up to root
    for (auto it = path.begin() + start_from_index; it != path.end(); ++it) {
      (*it)->UpdateInfo();
    }
  }
}

/**
 * @brief Updates only data-related subtree aggregators using a provided path.
 *
 * Used when only the node's data has changed, not its structure or keys.
 *
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <class Node>
inline void propagate_for_data_update_with_path(const std::vector<Node*>& path,
                                                size_t start_from_index = 0) {
  if constexpr (Node::TInfo::modify_data) {
    propagate_to_root_with_path<Node>(path, start_from_index);
  }
}

/**
 * @brief Updates only key-related subtree aggregators using a provided path.
 *
 * Used when only the node's keys have changed, not its data or structure.
 *
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <class Node>
inline void propagate_for_key_update_with_path(const std::vector<Node*>& path,
                                               size_t start_from_index = 0) {
  if constexpr (Node::TInfo::modify_keys) {
    propagate_to_root_with_path<Node>(path, start_from_index);
  }
}

/**
 * @brief Updates only tree structure-related subtree aggregators using a
 * provided path.
 *
 * Used when only the tree structure has changed, not its data or keys.
 *
 * @tparam Node The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <class Node>
inline void propagate_for_structure_update_with_path(
    const std::vector<Node*>& path, size_t start_from_index = 0) {
  if constexpr (Node::TInfo::modify_tree) {
    propagate_to_root_with_path<Node>(path, start_from_index);
  }
}

}  // namespace subtree_data
}  // namespace bst
