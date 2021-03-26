#pragma once

#include "common/base.h"

namespace bst {
template <class TTree, class TActionValue>
inline typename TTree::TNode* AddActionToSegment(
    typename TTree::TNode* root, size_t begin, size_t end,
    const TActionValue& action_value) {
  using TNode = typename TTree::TNode;
  TNode *l, *m, *r;
  TTree::SplitBySize(root, end, m, r);
  TTree::SplitBySize(m, begin, l, m);
  if (m) m->AddAction(action_value);
  return TTree::Join(TTree::Join(l, m), r);
}
}  // namespace bst
