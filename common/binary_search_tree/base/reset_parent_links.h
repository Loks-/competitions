#pragma once

#include "common/template.h"

#include <stack>

namespace bst {
namespace base {
namespace hidden {
template <class TNode>
inline void ResetParentLinksI(TNode* root, MetaFalse) {}

template <class TNode>
inline void ResetParentLinksI(TNode* root, MetaTrue) {
  root->p = nullptr;
  std::stack<TNode*> s;
  for (s.push(root); !s.empty();) {
    TNode* node = s.top();
    s.pop();
    if (node->l) {
      node->l->p = node;
      s.push(node->l);
    }
    if (node->r) {
      node->r->p = node;
      s.push(node->r);
    }
  }
}
}  // namespace hidden

template <class TNode>
inline void ResetParentLinks(TNode* root) {
  hidden::ResetParentLinksI(root, MetaBool<TNode::use_parent>());
}
}  // namespace base
}  // namespace bst
