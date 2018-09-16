#pragma once

#include "../../base.h"
#include <algorithm>

template<class TNode, class TCoordinate>
inline typename TNode::TInfo CSTGetSegmentI(const TNode* root, const TCoordinate& l, const TCoordinate& r)
{
    using TInfo = typename TNode::TInfo;
    if ((l <= root->info.xleft) && (r >= root->info.xright)) return root->info;
    assert(!root->IsLeaf());
    if (r < root->r->info.xleft)
        return CSTGetSegmentI(root->l, l, r);
    else if (l > root->l->info.xright)
        return CSTGetSegmentI(root->r, l, r);
    else
        return TInfo::MergeLR(CSTGetSegmentI(root->l, l, r), CSTGetSegmentI(root->r, l, r));
}

template<class TNode, class TCoordinate>
inline typename TNode::TInfo CSTGetSegment(const TNode* root, const TCoordinate& l, const TCoordinate& r)
{
    using TInfo = typename TNode::TInfo;
    static_assert(TInfo::has_segment, "has_segment should be true");
    if (r < l) return TInfo();
    assert(root);
    if ((r < root->info.xleft) || (l > root->info.xright)) return TInfo();
    return CSTGetSegmentI(root, l, r);
}
