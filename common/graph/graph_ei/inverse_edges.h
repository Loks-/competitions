#pragma once

#include "common/graph/graph_ei.h"

template <class TEdgeInfo>
inline DirectedGraphEI<TEdgeInfo> InverseEdges(
    const DirectedGraphEI<TEdgeInfo>& g) {
  DirectedGraphEI<TEdgeInfo> ginv(g.Size());
  for (unsigned u = 0; u < g.Size(); ++u) {
    for (const auto& e : g.EdgesEI(u)) {
      ginv.AddEdge(e.to, u, e.info);
    }
  }
  return ginv;
}
