#pragma once

#include "common/base.h"

namespace st {
namespace hidden {
template <class TNode>
inline typename TNode::TInfo GetSegmentInfoI(
    TNode* root, const typename TNode::TInfo::TCoordinate& l,
    const typename TNode::TInfo::TCoordinate& r) {
  using TInfo = typename TNode::TInfo;
  if ((l <= root->info.left) && (r >= root->info.right)) return root->info;
  assert(!root->IsLeaf());
  root->ApplyAction();
  if (r < root->r->info.left)
    return GetSegmentInfoI(root->l, l, r);
  else if (l > root->l->info.right)
    return GetSegmentInfoI(root->r, l, r);
  else
    return TInfo::MergeLR(GetSegmentInfoI(root->l, l, r),
                          GetSegmentInfoI(root->r, l, r));
}
}  // namespace hidden

template <class TNode>
inline typename TNode::TInfo GetSegmentInfo(
    TNode* root, const typename TNode::TInfo::TCoordinate& l,
    const typename TNode::TInfo::TCoordinate& r) {
  using TInfo = typename TNode::TInfo;
  static_assert(TInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r < l)) return TInfo();
  if ((r < root->info.left) || (l > root->info.right)) return TInfo();
  return hidden::GetSegmentInfoI(root, l, r);
}
}  // namespace st
