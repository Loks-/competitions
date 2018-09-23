#pragma once

#include "../../base.h"

template<class TNode>
inline typename TNode::TInfo STGetSegmentI(const TNode* root, const typename TNode::TInfo::TCoordinate& l, const typename TNode::TInfo::TCoordinate& r)
{
    using TInfo = typename TNode::TInfo;
    if ((l <= root->info.left) && (r >= root->info.right)) return root->info;
    assert(!root->IsLeaf());
    if (r < root->r->info.left)
        return STGetSegmentI(root->l, l, r);
    else if (l > root->l->info.right)
        return STGetSegmentI(root->r, l, r);
    else
        return TInfo::MergeLR(STGetSegmentI(root->l, l, r), STGetSegmentI(root->r, l, r));
}

template<class TNode>
inline typename TNode::TInfo STGetSegment(const TNode* root, const typename TNode::TInfo::TCoordinate& l, const typename TNode::TInfo::TCoordinate& r)
{
    using TInfo = typename TNode::TInfo;
    static_assert(TInfo::has_coordinate, "has_coordinate should be true");
    if (!root || (r < l)) return TInfo();
    if ((r < root->info.left) || (l > root->info.right)) return TInfo();
    return STGetSegmentI(root, l, r);
}
