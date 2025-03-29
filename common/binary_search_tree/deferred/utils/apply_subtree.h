#pragma once

namespace bst {
namespace deferred {

/**
 * @brief Applies deferred computations to all nodes in a subtree.
 *
 * This function traverses the subtree in a pre-order fashion and applies
 * deferred computations to each node. The order is:
 * 1. Apply deferred computations to the root node
 * 2. Recursively apply to left subtree
 * 3. Recursively apply to right subtree
 *
 * This order ensures that changes in a node's state are properly propagated
 * to its children before processing them.
 *
 * @tparam Node The BST node type.
 * @param root The root of the subtree to apply deferred computations to.
 */
template <typename Node>
constexpr void apply_subtree(Node* root) {
  if (!root) return;
  root->apply_deferred();
  if (root->left) apply_subtree(root->left);
  if (root->right) apply_subtree(root->right);
}

}  // namespace deferred
}  // namespace bst
