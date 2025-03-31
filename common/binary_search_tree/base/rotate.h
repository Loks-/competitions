#pragma once

#include "common/base.h"

namespace bst {
namespace base {
template <bool update_child, bool apply_action_to_child, class TNode>
inline void Rotate(TNode* child, TNode* parent, TNode* gparent) {
  assert(child && parent);
  if constexpr (apply_action_to_child) child->apply_deferred();
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

template <bool update_child, bool apply_action_to_node, class TNode>
inline bool RotateUp(TNode* node) {
  static_assert(TNode::has_parent, "has_parent should be true");
  if (!node || !node->parent) return false;
  Rotate<update_child, apply_action_to_node, TNode>(node, node->parent,
                                                    node->parent->parent);
  return true;
}
}  // namespace base
}  // namespace bst
