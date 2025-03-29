#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace base {

/**
 * @brief Gets the inorder index (position) of a node in the BST.
 *
 * This function calculates the position of a node in the inorder traversal
 * of the BST. The index is 0-based, meaning the leftmost node has index 0.
 * The index is calculated by counting the number of nodes in the left subtree
 * of each ancestor node when the current node is in the right subtree.
 *
 * Since this is a read-only operation that only traverses parent links,
 * no deferred computations are applied. If a pointer to a node is available,
 * it is assumed that all deferred computations have already been applied.
 *
 * @tparam Node The BST node type (automatically deduced as const or non-const).
 * @param node The node to calculate inorder index for (must not be nullptr).
 * @return The inorder index of the node (0 for leftmost node, 1 for second
 * node, etc.).
 */
template <class Node>
[[nodiscard]] constexpr unsigned index(const Node* node) {
  assert(node);
  static_assert(Node::has_parent, "Node must have parent pointer enabled");
  static_assert(Node::SubtreeDataType::has_size,
                "Subtree data should contain size");

  // Count nodes that come before the current node in inorder traversal
  unsigned count = bst::subtree_data::size(node->left);
  for (; node->parent; node = node->parent) {
    if (node->parent->right == node) {
      count += 1 + bst::subtree_data::size(node->parent->left);
    }
  }

  return count;
}

}  // namespace base
}  // namespace bst
