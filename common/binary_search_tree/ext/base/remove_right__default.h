#pragma once

#include "common/base.h"

#include <stack>

namespace bst {
namespace ext {
namespace base {
template <class TNode>
inline void RemoveRightDefault(TNode* root, TNode*& removed_node,
                               TNode*& old_parent) {
  thread_local std::stack<TNode*> s;
  assert(root && root->r);
  for (root->r->ApplyAction(); root->r->r; root->r->ApplyAction()) {
    s.push(root);
    root = root->r;
  }
  removed_node = root->r;
  root->set_right(removed_node->l);
  removed_node->ResetLinksAndUpdateInfo();
  old_parent = root;
  for (s.push(root); !s.empty(); s.pop())
    s.top()->RemoveOrUpdateInfo(removed_node);
}
}  // namespace base
}  // namespace ext
}  // namespace bst
