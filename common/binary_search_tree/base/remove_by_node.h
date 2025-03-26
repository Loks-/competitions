#pragma once

#include "common/binary_search_tree/base/remove_right.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"

namespace bst {
namespace base {
template <class TNode, bool apply_action>
inline TNode* RemoveByNode(TNode* node, TNode*& first_changed_node) {
  if (apply_action) bst::deferred::propagate_to_node(node);
  TNode *p = node->p, *c;
  if (node->l && node->r) {
    node->l->SetP(nullptr);
    TNode* l = RemoveRight<TNode>(node->l, c, first_changed_node);
    c->SetL(l);
    c->SetR(node->r);
    c->subtree_data.bti_copy(node);
    c->UpdateInfo();
    if (!first_changed_node) first_changed_node = c;
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
