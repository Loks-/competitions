#pragma once

namespace bst {
namespace base {

/**
 * @brief Gets the root node of the tree.
 *
 * This function traverses up the tree from a given node until it reaches
 * the root node (node with no parent). Since this is a read-only operation,
 * no deferred computations are applied.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction.
 *
 * @tparam Node The BST node type (automatically deduced as const or non-const).
 * @param node The node to start traversal from.
 * @return Pointer to the root node of the tree, or nullptr if the input node
 *         is nullptr.
 */
template <class Node>
[[nodiscard]] constexpr Node* root(Node* node) {
  if (!node) return nullptr;

  // Go up until we reach the root
  static_assert(Node::has_parent, "Node must have parent pointer enabled");
  while (node->parent) node = node->parent;

  return node;
}

}  // namespace base
}  // namespace bst
