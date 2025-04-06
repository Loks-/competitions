#pragma once

namespace tester {
namespace bst {

template <typename TNode>
[[nodiscard]] constexpr bool verify_parent_links(const TNode* root) {
  bool result = true;
  if (root->left)
    result = result && (root->left->parent == root) &&
             verify_parent_links(root->left);
  if (root->right)
    result = result && (root->right->parent == root) &&
             verify_parent_links(root->right);
  return result;
}

}  // namespace bst
}  // namespace tester
