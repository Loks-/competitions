#pragma once

#include "common/base.h"

namespace bst {
template <class TTree>
inline typename TTree::TNode* SegmentInfoByIndex(
    typename TTree::TNode* root, size_t begin, size_t end,
    typename TTree::TInfo& output) {
  using TNode = typename TTree::TNode;
  TNode *l, *m, *r;
  TTree::SplitBySize(root, end, m, r);
  TTree::SplitBySize(m, begin, l, m);
  if (m) output = m->info;
  return TTree::Join(TTree::Join(l, m), r);
}
}  // namespace bst
