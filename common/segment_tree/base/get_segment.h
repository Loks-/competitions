#pragma once

#include "common/base.h"
#include "common/segment_tree/segment.h"

namespace st {
namespace hidden {
template <class TNode>
inline Segment<TNode> GetSegmentI(TNode* root,
                                  const typename TNode::TInfo::TCoordinate& l,
                                  const typename TNode::TInfo::TCoordinate& r) {
  using TSegment = Segment<TNode>;
  if ((l <= root->info.left) && (r >= root->info.right)) return TSegment(root);
  if ((l > root->info.right) || (r < root->info.left)) return TSegment();
  assert(!root->IsLeaf());
  root->ApplyAction();
  if (r < root->r->info.left)
    return GetSegmentI(root->l, l, r);
  else if (l > root->l->info.right)
    return GetSegmentI(root->r, l, r);
  else
    return TSegment(GetSegmentI(root->l, l, r), GetSegmentI(root->r, l, r));
}
}  // namespace hidden

template <class TNode>
inline Segment<TNode> GetSegment(TNode* root,
                                 const typename TNode::TInfo::TCoordinate& l,
                                 const typename TNode::TInfo::TCoordinate& r) {
  using TInfo = typename TNode::TInfo;
  using TSegment = Segment<TNode>;
  static_assert(TInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r < l)) return TSegment();
  if ((r < root->info.left) || (l > root->info.right)) return TSegment();
  return hidden::GetSegmentI(root, l, r);
}
}  // namespace st
