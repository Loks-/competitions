#pragma once

#include "../../base.h"

template<class TNode>
inline typename TNode::TInfo STGetSegmentI(const TNode* root, unsigned begin, unsigned end)
{
    using TInfo = typename TNode::TInfo;
    if ((begin == 0) && (end == root->info.GetSize()) return root->info;
    assert(!root->IsLeaf());
    unsigned m = root->l->info.GetSize();
    if (end <= m)
        return STGetSegmentI(root->l, begin, end);
    else if (begin >= m)
        return STGetSegmentI(root->r, begin - m, end - m);
    else
        return TInfo::MergeLR(STGetSegmentI(root->l, begin, m), STGetSegmentI(root->r, m, end));
}

template<class TNode>
inline typename TNode::TInfo STGetSegment(const TNode* root, unsigned begin, unsigned end)
{
    using TInfo = typename TNode::TInfo;
    static_assert(TInfo::has_size, "has_size should be true");
    if (begin >= end) return TInfo();
    assert(root);
    end = std::min(end, root->info.GetSize());
    if (begin >= end) return TInfo();
    return STGetSegmentI(root, begin, end);
}
