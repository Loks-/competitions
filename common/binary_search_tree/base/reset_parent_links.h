#pragma once

#include "../../template.h"
#include <stack>

template <class TNode>
inline void ResetParentLinksI(TNode* root, TFakeFalse) {}

template <class TNode>
inline void ResetParentLinksI(TNode* root, TFakeTrue) {
  root->p = 0;
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

template <class TNode>
inline void ResetParentLinks(TNode* root) {
  ResetParentLinksI(root, TFakeBool<TNode::use_parent>());
}
