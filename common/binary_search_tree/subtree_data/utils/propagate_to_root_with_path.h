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
 * @tparam TNode The BST node type.
 * @param path Vector containing the path from node to root (inclusive).
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename TNode>
constexpr void propagate_to_root_with_path(const std::vector<TNode*>& path,
                                           size_t start_from_index = 0) {
  if constexpr (!TNode::SubtreeDataType::empty) {
    // Update subtree data from node up to root
    for (auto it = path.begin() + start_from_index; it != path.end(); ++it)
      (*it)->update_subtree_data();
  }
}

}  // namespace subtree_data
}  // namespace bst
