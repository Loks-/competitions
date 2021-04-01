#pragma once

#include "common/base.h"

namespace bst {
template <class TTree, class TActionValue>
inline typename TTree::TNode* AddActionToSegmentByKey(
    typename TTree::TNode* root,
    const typename TTree::TKey& begin,
    const typename TTree::TKey& end,
    const TActionValue& action_value) {
  using TNode = typename TTree::TNode;
  TNode *l, *m, *r;
  TTree::SplitByKey(root, end, m, r);
  TTree::SplitByKey(m, begin, l, m);
  if (m) m->AddAction(action_value);
  return TTree::Join(TTree::Join(l, m), r);
}
}  // namespace bst
