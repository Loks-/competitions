#pragma once

#include "common/base.h"

namespace bst {
namespace base {
template <class TNode, bool update_child, bool apply_action>
inline void Rotate(TNode* child, TNode* parent, TNode* gparent) {
  assert(child && parent);
  if (apply_action) {
    parent->apply_deferred();
    child->apply_deferred();
  }
  if (gparent) {
    if (gparent->left == parent) {
      gparent->left = child;
    } else {
      gparent->right = child;
    }
  }
  child->set_parent(gparent);
  if (parent->left == child) {
    parent->set_left(child->right);
    child->set_right(parent);
  } else {
    parent->set_right(child->left);
    child->set_left(parent);
  }
  parent->update_subtree_data();
  if (update_child) child->update_subtree_data();
}

template <class TNode, bool update_child, bool apply_action>
inline bool RotateUp(TNode* node) {
  static_assert(TNode::has_parent, "has_parent should be true");
  if (!node || !node->parent) return false;
  Rotate<TNode, update_child, apply_action>(node, node->parent,
                                            node->parent->parent);
  return true;
}
}  // namespace base
}  // namespace bst
