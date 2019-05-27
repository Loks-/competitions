#pragma once

namespace bst {
template <class TTree>
inline typename TTree::TNode* SegmentInfoByKey(
    typename TTree::TNode* root, const typename TTree::TKey& begin,
    const typename TTree::TKey& end, typename TTree::TInfo& output) {
  using TNode = typename TTree::TNode;
  TNode *l, *m, *r;
  TTree::SplitByKey(root, end, m, r);
  TTree::SplitByKey(m, begin, l, m);
  if (m) output = m->info;
  return TTree::Join(TTree::Join(l, m), r);
}
}  // namespace bst
