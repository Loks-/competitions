#pragma once

#include "../../base.h"

template <class TNode, class TActionValue>
inline void STAddActionToSegmentI(TNode* root,
                                  const typename TNode::TInfo::TCoordinate& l,
                                  const typename TNode::TInfo::TCoordinate& r,
                                  const TActionValue& action_value) {
  if ((l <= root->info.left) && (r >= root->info.right)) {
    root->AddAction(action_value);
  } else {
    assert(!root->IsLeaf());
    root->ApplyAction();
    if (l <= root->l->info.right)
      STAddActionToSegmentI(root->l, l, r, action_value);
    if (r >= root->r->info.left)
      STAddActionToSegmentI(root->r, l, r, action_value);
    root->UpdateInfo();
  }
}

template <class TNode, class TActionValue>
inline void STAddActionToSegment(TNode* root,
                                 const typename TNode::TInfo::TCoordinate& l,
                                 const typename TNode::TInfo::TCoordinate& r,
                                 const TActionValue& action_value) {
  using TInfo = typename TNode::TInfo;
  static_assert(TInfo::has_coordinate, "has_coordinate should be true");
  if (!root || (r < l)) return;
  if ((r < root->info.left) || (l > root->info.right)) return;
  STAddActionToSegmentI(root, l, r, action_value);
}
