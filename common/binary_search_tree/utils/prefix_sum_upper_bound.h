#pragma once

#include "common/base.h"

namespace bst {
template <class TSTDSumAggregator, class TNode, class TValue>
inline TNode* PrefixSumUpperBound(TNode* root, TValue value) {
  if (!root) return nullptr;
  if (value >= TSTDSumAggregator::get(root)) return nullptr;
  for (TNode* node = root; node;) {
    node->apply_deferred();
    TValue ls = TSTDSumAggregator::get(node->left);
    if (value < ls) {
      node = node->left;
    } else if (value < ls + node->data) {
      return node;
    } else {
      value -= (ls + node->data);
      node = node->right;
    }
  }
  assert(false);
  return nullptr;
}
}  // namespace bst
