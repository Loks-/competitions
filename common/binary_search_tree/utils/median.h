#pragma once

#include "../base/find_by_order.h"
#include "../../base.h"

template <class TNode, class TValue>
inline TValue BSTMedian(TNode* root) {
  static_assert(TNode::TInfo::has_size, "info should contain size");
  assert(root);
  unsigned size = root->info.size;
  if (size & 1)
    return TValue(BSTFindByOrder(root, size / 2)->key);
  else
    return TValue(BSTFindByOrder(root, size / 2 - 1)->key +
                  BSTFindByOrder(root, size / 2)->key) /
           TValue(2);
}
