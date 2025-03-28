#pragma once

#include "common/binary_search_tree/base/remove_right.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"

namespace bst {
namespace base {
template <class TNode, bool apply_action>
inline TNode* RemoveByNode(TNode* node, TNode*& first_changed_node) {
  if (apply_action) bst::deferred::propagate_to_node(node);
  TNode *p = node->parent, *c;
  if (node->left && node->right) {
    node->left->set_parent(nullptr);
    TNode* l = RemoveRight<TNode>(node->left, c, first_changed_node);
    c->set_left(l);
    c->set_right(node->right);
    c->subtree_data.bti_copy(node);
    c->update_subtree_data();
    if (!first_changed_node) first_changed_node = c;
  } else {
    first_changed_node = p;
    c = (node->left ? node->left : node->right);
    if (c) c->set_parent(p);
  }
  node->reset_links_and_update_subtree_data();
  if (!p) return c;
  if (node == p->left) {
    p->set_left(c);
  } else {
    p->set_right(c);
  }
  for (; p; p = (c = p)->parent) p->update_subtree_data();
  return c;
}
}  // namespace base
}  // namespace bst
