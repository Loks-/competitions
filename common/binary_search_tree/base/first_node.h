#pragma once

namespace bst {
template <class TNode>
inline TNode* FirstNode(TNode* root) {
  if (!root) return nullptr;
  for (; root->l;) root = root->l;
  return root;
}
}  // namespace bst
