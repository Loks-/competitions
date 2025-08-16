#pragma once

namespace bst {
namespace subtree_data {

/**
 * @brief Updates subtree data from a node up to the root.
 *
 * This function ensures that all subtree data aggregators are updated along the
 * path from the given node to the root. It's used when node's data, keys,
 * structure, or node order has changed and we need to propagate these changes
 * upward.
 *
 * The updates are performed from the node up to the root to ensure that each
 * node's subtree data is updated with the latest changes from its children.
 *
 * @tparam TNode The BST node type.
 * @param node The starting node to update from.
 */
template <typename TNode>
constexpr void propagate_to_root(TNode* node) {
  if constexpr (!TNode::SubtreeDataType::empty) {
    static_assert(TNode::has_parent, "Node must have parent pointer enabled");

    // Update subtree data from node up to root
    for (; node; node = node->parent) node->update_subtree_data();
  }
}

}  // namespace subtree_data
}  // namespace bst
