#pragma once

#include "common/binary_search_tree/base/left.h"

namespace bst {
namespace base {

/**
 * @brief Gets the next node in inorder traversal.
 *
 * This function finds the next node in inorder traversal (successor) of a BST.
 * The successor is the node with the smallest key that is larger than the
 * current node's key. It can optionally apply deferred computations to ensure
 * the node's state is up to date.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction. When called with a const node and
 * apply_deferred=false, no deferred computations will be applied.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @param node The current node to find successor for.
 * @return Pointer to the next node in inorder traversal, or nullptr if there
 *         is no successor (current node is the rightmost node).
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* next(TNode* node) {
  if (!node) return nullptr;

  // Apply deferred computations if requested
  if constexpr (apply_deferred) {
    node->apply_deferred();
  }

  // If node has right child, successor is the leftmost node in right subtree
  if (node->right) return left<apply_deferred>(node->right);

  // Otherwise, go up until we find a node that is a left child
  static_assert(TNode::has_parent, "Node must have parent pointer enabled");
  while (node->parent && (node->parent->right == node)) node = node->parent;

  return node->parent;
}

}  // namespace base
}  // namespace bst
