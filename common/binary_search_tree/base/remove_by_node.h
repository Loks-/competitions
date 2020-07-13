#pragma once

#include "common/binary_search_tree/action/apply_root_to_node.h"
#include "common/binary_search_tree/base/remove_right.h"

namespace bst {
namespace base {
template <class TNode, bool apply_action>
inline TNode* RemoveByNode(TNode* node, TNode*& first_changed_node) {
  if (apply_action) bst::action::ApplyRootToNode(node);
  TNode *p = node->p, *c;
  if (node->l && node->r) {
    node->l->SetP(nullptr);
    TNode* l = RemoveRight<TNode>(node->l, c, first_changed_node);
    c->SetL(l);
    c->SetR(node->r);
    c->UpdateInfo();
  } else {
    first_changed_node = p;
    c = (node->l ? node->l : node->r);
    if (c) c->SetP(p);
  }
  node->ResetLinksAndUpdateInfo();
  if (!p) return c;
  if (node == p->l)
    p->SetL(c);
  else
    p->SetR(c);
  for (; p; p = (c = p)->p) p->UpdateInfo();
  return c;
}
}  // namespace base
}  // namespace bst
