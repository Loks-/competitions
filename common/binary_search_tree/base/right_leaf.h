#pragma once

namespace bst {
namespace base {

/**
 * @brief Gets the rightmost leaf node in a subtree.
 *
 * This function traverses the rightmost path of a subtree to find the leaf node
 * with the largest key. A leaf node is a node that has no children (both left
 * and right pointers are nullptr). It can optionally apply deferred
 * computations to ensure the node's state is up to date.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction. When called with a const node and
 * apply_deferred=false, no deferred computations will be applied.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam Node The BST node type (automatically deduced as const or non-const).
 * @param root The root of the subtree to search in.
 * @return Pointer to the rightmost leaf node, or nullptr if the subtree is
 * empty.
 */
template <bool apply_deferred = true, typename Node>
[[nodiscard]] constexpr Node* right_leaf(Node* root) {
  if (!root) return nullptr;

  // Traverse rightmost path until we find a leaf
  while (true) {
    // Apply deferred computations if requested
    if constexpr (apply_deferred) {
      root->apply_deferred();
    }

    if (root->right) {
      root = root->right;
    } else if (root->left) {
      root = root->left;
    } else {
      break;
    }
  }

  return root;
}

}  // namespace base
}  // namespace bst
