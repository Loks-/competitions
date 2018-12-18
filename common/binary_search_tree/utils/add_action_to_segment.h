#pragma once

template <class TTree, class TActionValue>
inline typename TTree::TNode* BSTAddActionToSegment(
    typename TTree::TNode* root, unsigned begin, unsigned end,
    const TActionValue& action_value) {
  using TNode = typename TTree::TNode;
  TNode *l, *m, *r;
  TTree::SplitBySize(root, end, m, r);
  TTree::SplitBySize(m, begin, l, m);
  m->AddAction(action_value);
  return TTree::Join(TTree::Join(l, m), r);
}
