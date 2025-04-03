#pragma once

namespace bst {
namespace base {

/**
 * @brief Checks if a node is a leaf node.
 *
 * A leaf node is a node that has no children (both left and right pointers
 * are nullptr). This function can optionally apply deferred computations
 * to ensure the node's state is up to date before checking.
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
 * @param node The node to check.
 * @return true if the node is a leaf (has no children), false otherwise.
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr bool leaf(TNode* node) {
  if (!node) return false;

  // Apply deferred computations if requested
  if constexpr (apply_deferred) node->apply_deferred();

  return !node->left && !node->right;
}

}  // namespace base
}  // namespace bst
