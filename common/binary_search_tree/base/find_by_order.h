#pragma once

#include "../../base.h"

template <class TNode>
inline TNode* BSTFindByOrder(TNode* root, unsigned order_index) {
  static_assert(TNode::TInfo::has_size, "info should contain size");
  if (!root) return 0;
  if (order_index >= root->info.size) return 0;
  for (TNode* node = root; node;) {
    node->ApplyAction();
    unsigned ls = (node->l ? node->l->info.size : 0);
    if (order_index < ls) {
      node = node->l;
    } else if (order_index == ls) {
      return node;
    } else {
      order_index -= 1 + ls;
      node = node->r;
    }
  }
  assert(false);
  return 0;
}
