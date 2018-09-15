#pragma once

#include "../../base.h"
#include <vector>

template <class TSegmentTree, class TCoordinate>
inline typename TSegmentTree::TNode* CSTBuildTreeI(TSegmentTree& st, const std::vector<typename TSegmentTree::TData>& vdata, const std::vector<TCoordinate>& vx, unsigned first, unsigned last)
{
    using TNode = typename TSegmentTree::TNode;
    if (first >= last) return 0;
    TNode* root = 0;
    if (first + 1 == last)
    {
        root = st.GetNewNode(vdata[first]);
        root->info.SetCoordinates(vx[first]);
    }
    else
    {
        root = st.GetNewNode();
        unsigned m = (first + last) / 2;
        root->SetL(CSTBuildTreeI(st, vdata, vx, first, m));
        root->SetR(CSTBuildTreeI(st, vdata, vx, m + 1, last));
    }
    root->UpdateInfo();
    return root;
}

template <class TSegmentTree, class TCoordinate>
inline typename TSegmentTree::TNode* CSTBuildTree(TSegmentTree& st, const std::vector<typename TSegmentTree::TData>& vdata, const std::vector<TCoordinate>& vx)
{
    using TInfo = typename TSegmentTree::TInfo;
    static_assert(TInfo::has_segment, "has_segment should be true");
    assert(vdata.size() == vx.size());
    return CSTBuildTreeI(st, vnodes, vx, 0, unsigned(vdata.size()));
}
