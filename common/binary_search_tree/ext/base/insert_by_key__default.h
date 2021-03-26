#pragma once

#include <stack>

namespace bst {
namespace ext {
namespace base {
template <class TNode>
inline void InsertByKeyDefault(TNode* root, TNode* node) {
  thread_local std::stack<TNode*> s;
  for (;;) {
    s.push(root);
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
  for (; !s.empty(); s.pop()) s.top()->UpdateInfo();
}
}  // namespace base
}  // namespace ext
}  // namespace bst
