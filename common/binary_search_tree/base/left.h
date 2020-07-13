#pragma once

namespace bst {
template <class TNode>
inline TNode* Left(TNode* root) {
  if (!root) return nullptr;
  for (; root->l;) root = root->l;
  return root;
}
}  // namespace bst
