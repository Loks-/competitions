#pragma once

#include "common/base.h"

namespace bst {
namespace base {
template <class TNode, bool update_child, bool apply_action>
inline void Rotate(TNode* child, TNode* parent, TNode* gparent) {
  assert(child && parent);
  if (apply_action) {
    parent->ApplyAction();
    child->ApplyAction();
  }
  if (gparent) {
    if (gparent->l == parent)
      gparent->l = child;
    else
      gparent->r = child;
  }
  child->SetP(gparent);
  if (parent->l == child) {
    parent->SetL(child->r);
    child->SetR(parent);
  } else {
    parent->SetR(child->l);
    child->SetL(parent);
  }
  parent->UpdateInfo();
  if (update_child) child->UpdateInfo();
}

template <class TNode, bool update_child, bool apply_action>
inline bool RotateUp(TNode* node) {
  static_assert(TNode::use_parent, "use_parent should be true");
  if (!node || !node->p) return false;
  Rotate<TNode, update_child, apply_action>(node, node->p, node->p->p);
  return true;
}
}  // namespace base
}  // namespace bst
