#pragma once

namespace bst {
template <class TNode>
inline TNode* Right(TNode* root) {
  if (!root) return nullptr;
  for (; root->r;) root = root->r;
  return root;
}
}  // namespace bst
