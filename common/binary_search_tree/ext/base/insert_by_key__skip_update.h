#pragma once

namespace bst {
namespace ext {
namespace base {
template <class TNode>
inline void InsertByKeySkipUpdate(TNode* root, TNode* node) {
  for (;;) {
    root->ApplyAction();
    if (root->key < node->key) {
      if (root->r) {
        root = root->r;
      } else {
        root->SetR(node);
        break;
      }
    } else {
      if (root->l) {
        root = root->l;
      } else {
        root->SetL(node);
        break;
      }
    }
  }
}
}  // namespace base
}  // namespace ext
}  // namespace bst
