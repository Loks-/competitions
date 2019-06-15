#pragma once

#include "common/base.h"

namespace st {
template <class TNode>
inline typename TNode::TInfo GetSegmentI(
    TNode* root, const typename TNode::TInfo::TCoordinate& l,
    const typename TNode::TInfo::TCoordinate& r) {
  using TInfo = typename TNode::TInfo;
  if ((l <= root->info.left) && (r >= root->info.right)) return root->info;
  assert(!root->IsLeaf());
  root->ApplyAction();
  if (r < root->r->info.left)
    return GetSegmentI(root->l, l, r);
  else if (l > root->l->info.right)
    return GetSegmentI(root->r, l, r);
  else
    return TInfo::MergeLR(GetSegmentI(root->l, l, r),
                          GetSegmentI(root->r, l, r));
}

template <class TNode>
inline typename TNode::TInfo GetSegment(
    TNode* root, const typename TNode::TInfo::TCoordinate& l,
    const typename TNode::TInfo::TCoordinate& r) {
  using TInfo = typename TNode::TInfo;
  static_assert(TInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r < l)) return TInfo();
  if ((r < root->info.left) || (l > root->info.right)) return TInfo();
  return GetSegmentI(root, l, r);
}
}  // namespace st
