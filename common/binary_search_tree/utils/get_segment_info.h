#pragma once

namespace bst {
template <class TTree>
inline typename TTree::TNode* GetSegmentInfo(typename TTree::TNode* root,
                                             unsigned begin, unsigned end,
                                             typename TTree::TInfo& output) {
  using TNode = typename TTree::TNode;
  TNode *l, *m, *r;
  TTree::SplitBySize(root, end, m, r);
  TTree::SplitBySize(m, begin, l, m);
  output = m->info;
  return TTree::Join(TTree::Join(l, m), r);
}
}  // namespace bst
