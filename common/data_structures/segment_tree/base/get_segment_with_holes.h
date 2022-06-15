#pragma once

#include "common/base.h"
#include "common/data_structures/segment_tree/segment.h"

namespace ds {
namespace st {
namespace hidden {
template <class TNode>
inline Segment<TNode> GetSegmentWithHolesI(
    TNode* root, const typename TNode::TCoordinate& l,
    const typename TNode::TCoordinate& r) {
  using TSegment = Segment<TNode>;
  if ((l <= root->sinfo.left) && (r >= root->sinfo.right))
    return TSegment(root);
  if ((l >= root->sinfo.right) || (r <= root->sinfo.left)) return TSegment();
  assert(!root->IsLeaf());
  root->ApplyAction();
  if (r <= root->l->sinfo.right)
    return GetSegmentWithHolesI(root->l, l, r);
  else if (l >= root->r->sinfo.left)
    return GetSegmentWithHolesI(root->r, l, r);
  else
    return TSegment(GetSegmentWithHolesI(root->l, l, r),
                    GetSegmentWithHolesI(root->r, l, r));
}
}  // namespace hidden

template <class TNode>
inline Segment<TNode> GetSegmentWithHoles(
    TNode* root, const typename TNode::TCoordinate& l,
    const typename TNode::TCoordinate& r) {
  using TSegment = Segment<TNode>;
  static_assert(TNode::TSInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r <= l) || (r <= root->sinfo.left) || (l >= root->sinfo.right))
    return TSegment();
  return hidden::GetSegmentWithHolesI(root, l, r);
}
}  // namespace st
}  // namespace ds
