#pragma once

namespace bst {
namespace base {

/**
 * @brief Gets the leftmost node in a subtree.
 *
 * This function traverses the leftmost path of a subtree to find the node
 * with the smallest key. It can optionally apply deferred computations
 * to ensure the node's state is up to date.
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
 * @param root The root of the subtree to search in.
 * @return Pointer to the leftmost node, or nullptr if the subtree is empty.
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* left(TNode* root) {
  if (!root) return nullptr;

  // Apply deferred computations if requested
  if constexpr (apply_deferred) root->apply_deferred();

  // Traverse leftmost path
  while (root->left) {
    root = root->left;
    if constexpr (apply_deferred) root->apply_deferred();
  }

  return root;
}

}  // namespace base
}  // namespace bst
