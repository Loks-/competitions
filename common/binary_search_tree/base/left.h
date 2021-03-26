#pragma once

namespace bst {
namespace base {
template <class TNode>
inline TNode* Left(TNode* root) {
  if (!root) return nullptr;
  for (; root->l;) root = root->l;
  return root;
}
}  // namespace base
}  // namespace bst
