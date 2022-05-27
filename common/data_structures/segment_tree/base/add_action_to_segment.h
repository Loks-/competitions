#pragma once

#include "common/base.h"

namespace ds {
namespace st {
namespace hidden {
template <class TNode, class TActionValue>
inline void AddActionToSegmentI(TNode* root,
                                const typename TNode::TCoordinate& l,
                                const typename TNode::TCoordinate& r,
                                const TActionValue& action_value) {
  if ((l <= root->sinfo.left) && (r >= root->sinfo.right)) {
    root->AddAction(action_value);
  } else {
    assert(!root->IsLeaf());
    root->ApplyAction();
    if (l <= root->l->sinfo.right)
      AddActionToSegmentI(root->l, l, r, action_value);
    if (r >= root->r->sinfo.left)
      AddActionToSegmentI(root->r, l, r, action_value);
    root->UpdateInfo();
  }
}
}  // namespace hidden

template <class TNode, class TActionValue>
inline void AddActionToSegment(TNode* root,
                               const typename TNode::TCoordinate& l,
                               const typename TNode::TCoordinate& r,
                               const TActionValue& action_value) {
  static_assert(TNode::TSInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r < l)) return;
  if ((r < root->sinfo.left) || (l > root->sinfo.right)) return;
  hidden::AddActionToSegmentI(root, l, r, action_value);
}
}  // namespace st
}  // namespace ds
