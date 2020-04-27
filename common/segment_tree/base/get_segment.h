#pragma once

#include "common/base.h"
#include "common/segment_tree/segment.h"

namespace st {
namespace hidden {
template <class TNode>
inline Segment<TNode> GetSegmentI(TNode* root,
                                  const typename TNode::TCoordinate& l,
                                  const typename TNode::TCoordinate& r) {
  using TSegment = Segment<TNode>;
  if ((l <= root->sinfo.left) && (r >= root->sinfo.right))
    return TSegment(root);
  if ((l > root->sinfo.right) || (r < root->sinfo.left)) return TSegment();
  assert(!root->IsLeaf());
  root->ApplyAction();
  if (r < root->r->sinfo.left)
    return GetSegmentI(root->l, l, r);
  else if (l > root->l->sinfo.right)
    return GetSegmentI(root->r, l, r);
  else
    return TSegment(GetSegmentI(root->l, l, r), GetSegmentI(root->r, l, r));
}
}  // namespace hidden

template <class TNode>
inline Segment<TNode> GetSegment(TNode* root,
                                 const typename TNode::TCoordinate& l,
                                 const typename TNode::TCoordinate& r) {
  using TSegment = Segment<TNode>;
  static_assert(TNode::TSInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r < l)) return TSegment();
  if ((r < root->sinfo.left) || (l > root->sinfo.right)) return TSegment();
  return hidden::GetSegmentI(root, l, r);
}
}  // namespace st
