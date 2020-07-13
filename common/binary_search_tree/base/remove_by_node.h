#pragma once

#include "common/binary_search_tree/base/remove_right.h"

#include <stack>

namespace bst {
// Assume that action is applied from root to node
template <class TNode>
inline TNode* RemoveByNodeI(TNode* node) {
  TNode *p = node->p, *c;
  if (node->l && node->r) {
    TNode* l = RemoveRight<TNode>(node->l, c);
    c->SetL(l);
    c->SetR(node->r);
    c->UpdateInfo();
  } else {
    c = (node->l ? node->l : node->r);
    if (c) c->SetP(p);
  }
  node->ResetLinksAndUpdateInfo();
  if (!p) return c;
  if (node == p->l)
    p->SetL(c);
  else
    p->SetR(c);
  for (; p; p = (c = p)->p) p->RemoveOrUpdateInfo(node);
  return c;
}
}  // namespace bst
