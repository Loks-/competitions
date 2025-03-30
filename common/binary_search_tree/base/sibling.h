#pragma once

namespace bst {
namespace base {

/**
 * @brief Gets the sibling of a node.
 *
 * This function finds the sibling of a given node using its parent node.
 * The sibling is the other child of the parent node (if the current node
 * is the left child, the sibling is the right child, and vice versa).
 * Since this is a read-only operation, no deferred computations are applied.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction.
 *
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @param node The node to find sibling for.
 * @param parent The parent node of the given node.
 * @return Pointer to the sibling node, or nullptr if the node is the only
 *         child of its parent.
 */
template <typename TNode>
[[nodiscard]] constexpr TNode* sibling(TNode* node, TNode* parent) {
  return parent ? (parent->left == node ? parent->right : parent->left)
                : nullptr;
}

}  // namespace base
}  // namespace bst
