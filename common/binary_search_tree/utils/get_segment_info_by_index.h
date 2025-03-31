#pragma once

#include "common/base.h"

namespace bst {
template <class TTree, class TNode>
inline TNode* GetSegmentInfoByIndex(TNode* root, size_t begin, size_t end,
                                    typename TTree::SubtreeDataType& output) {
  TNode *l, *m, *r;
  TTree::split_at(root, end, m, r);
  TTree::split_at(m, begin, l, m);
  if (m) output = m->subtree_data;
  return TTree::join(TTree::join(l, m), r);
}
}  // namespace bst
