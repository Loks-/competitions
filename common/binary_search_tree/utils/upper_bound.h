#pragma once

#include "common/base.h"

namespace bst {
template <class TNode, class TValue>
inline TNode* PrefixSumUpperBound(TNode* root, TValue value) {
  if (!root) return nullptr;
  if (value >= root->info.sum) return nullptr;
  for (TNode* node = root; node;) {
    node->ApplyAction();
    TValue ls = (node->l ? node->l->info.sum : 0);
    if (value < ls) {
      node = node->l;
    } else if (value < ls + node->data) {
      return node;
    } else {
      value -= (ls + node->data);
      node = node->r;
    }
  }
  assert(false);
  return nullptr;
}
}  // namespace bst
