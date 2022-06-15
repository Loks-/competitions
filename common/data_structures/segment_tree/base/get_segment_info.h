#pragma once

#include "common/base.h"
#include "common/data_structures/segment_tree/info/merge.h"

namespace ds {
namespace st {
namespace hidden {
template <class TNode>
inline typename TNode::TInfo GetSegmentInfoI(
    TNode* root, const typename TNode::TCoordinate& l,
    const typename TNode::TCoordinate& r) {
  if ((l <= root->sinfo.left) && (r >= root->sinfo.right)) return root->info;
  assert(!root->IsLeaf());
  root->ApplyAction();
  if (r <= root->l->sinfo.right)
    return GetSegmentInfoI(root->l, l, r);
  else if (l >= root->r->sinfo.left)
    return GetSegmentInfoI(root->r, l, r);
  else
    return info::MergeLR(GetSegmentInfoI(root->l, l, r),
                         GetSegmentInfoI(root->r, l, r));
}
}  // namespace hidden

template <class TNode>
inline typename TNode::TInfo GetSegmentInfo(
    TNode* root, const typename TNode::TCoordinate& l,
    const typename TNode::TCoordinate& r) {
  using TInfo = typename TNode::TInfo;
  static_assert(TNode::TSInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r <= l) || (r <= root->sinfo.left) || (l >= root->sinfo.right))
    return TInfo();
  return hidden::GetSegmentInfoI(root, l, r);
}
}  // namespace st
}  // namespace ds
