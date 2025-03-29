#pragma once

#include "common/binary_search_tree/base/right_leaf.h"

namespace bst {
namespace base {

/**
 * @brief Gets the previous leaf node in inorder traversal.
 *
 * This function finds the previous leaf node in inorder traversal of a BST.
 * The previous leaf is the leaf node with the largest key that is smaller than
 * the current node's key. It can optionally apply deferred computations
 * to ensure the node's state is up to date.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction. When called with a const node and
 * apply_deferred=false, no deferred computations will be applied.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam Node The BST node type (automatically deduced as const or non-const).
 * @param node The current node to find previous leaf for.
 * @return Pointer to the previous leaf node in inorder traversal, or nullptr if
 *         there is no previous leaf (current node is the leftmost leaf).
 */
template <bool apply_deferred = true, class Node>
[[nodiscard]] constexpr Node* prev_leaf(Node* node) {
  if (!node) return nullptr;

  // Apply deferred computations if requested
  if constexpr (apply_deferred) {
    node->apply_deferred();
  }

  // If node has left child, previous leaf is the rightmost leaf in left subtree
  if (node->left) return right_leaf<apply_deferred>(node->left);

  // Otherwise, go up until we find a node that is a right child
  static_assert(Node::has_parent, "Node must have parent pointer enabled");
  while (node->parent && (node->parent->left == node)) node = node->parent;
  node = node->parent;

  if (node && node->left) {
    return right_leaf<apply_deferred>(node->left);
  } else {
    return nullptr;
  }
}

}  // namespace base
}  // namespace bst
