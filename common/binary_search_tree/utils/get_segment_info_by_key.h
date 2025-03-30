#pragma once

namespace bst {
template <class TTree>
inline typename TTree::TNode* GetSegmentInfoByKey(
    typename TTree::TNode* root, const typename TTree::KeyType& begin,
    const typename TTree::KeyType& end,
    typename TTree::SubtreeDataType& output) {
  using TNode = typename TTree::TNode;
  TNode *l, *m, *r;
  TTree::split(root, end, m, r);
  TTree::split(m, begin, l, m);
  if (m) output = m->subtree_data;
  return TTree::join(TTree::join(l, m), r);
}
}  // namespace bst
