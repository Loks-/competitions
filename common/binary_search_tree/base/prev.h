#pragma once

#include "common/binary_search_tree/base/right.h"

namespace bst {
namespace base {

/**
 * @brief Gets the previous node in inorder traversal.
 *
 * This function finds the previous node in inorder traversal (predecessor) of a
 * BST. The predecessor is the node with the largest key that is smaller than
 * the current node's key. It can optionally apply deferred computations to
 * ensure the node's state is up to date.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction. When called with a const node and
 * apply_deferred=false, no deferred computations will be applied.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam Node The BST node type (automatically deduced as const or non-const).
 * @param node The current node to find predecessor for.
 * @return Pointer to the previous node in inorder traversal, or nullptr if
 * there is no predecessor (current node is the leftmost node).
 */
template <bool apply_deferred = true, class Node>
[[nodiscard]] constexpr Node* prev(Node* node) {
  if (!node) return nullptr;

  // Apply deferred computations if requested
  if constexpr (apply_deferred) {
    node->apply_deferred();
  }

  // If node has left child, predecessor is the rightmost node in left subtree
  if (node->left) return right<apply_deferred>(node->left);

  // Otherwise, go up until we find a node that is a right child
  static_assert(Node::has_parent, "Node must have parent pointer enabled");
  while (node->parent && (node->parent->left == node)) node = node->parent;

  return node->parent;
}

}  // namespace base
}  // namespace bst
