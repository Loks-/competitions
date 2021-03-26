#pragma once

namespace bst {
namespace base {
template <class TNode>
inline TNode* Right(TNode* root) {
  if (!root) return nullptr;
  for (; root->r;) root = root->r;
  return root;
}
}  // namespace base
}  // namespace bst
