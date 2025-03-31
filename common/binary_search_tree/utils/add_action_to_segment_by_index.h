#pragma once

#include "common/base.h"

namespace bst {
template <class TDeferred, class TTree, class TNode, class TActionValue>
inline TNode* AddActionToSegmentByIndex(TNode* root, size_t begin, size_t end,
                                        const TActionValue& action_value) {
  TNode *l, *m, *r;
  TTree::split_at(root, end, m, r);
  TTree::split_at(m, begin, l, m);
  if (m) TDeferred::add(m, action_value);
  return TTree::join(TTree::join(l, m), r);
}
}  // namespace bst
