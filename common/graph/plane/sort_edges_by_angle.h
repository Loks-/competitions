#pragma once

#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/vector.h"
#include "common/graph/edge.h"
#include "common/graph/graph_ei.h"
#include <algorithm>
#include <vector>

template <class TEdgeInfo>
inline void SortEdgesByAngle(UndirectedGraphEI<TEdgeInfo>& g) {
  auto cmp = [](const graph::Edge<TEdgeInfo>& e1,
                const graph::Edge<TEdgeInfo>& e2) {
    return CompareVectorAngle(e1.info.v, e2.info.v);
  };
  std::vector<graph::Edge<TEdgeInfo>> ve;
  for (unsigned i = 0; i < g.Size(); ++i) {
    ve.clear();
    for (const auto& e : g.EdgesEI(i)) ve.push_back(e);
    std::sort(ve.begin(), ve.end(), cmp);
    g.ClearEdges(i);
    for (const auto& e : ve) g.AddBaseEdge(i, e.to, e.info);
  }
}
