#pragma once

#include "common/graph/graph_ei.h"

template <class TEdgeInfoInput, class TEdgeInfoOutput, class TFunction>
inline DirectedGraphEI<TEdgeInfoOutput> ReplaceEdgeInfo(
    const DirectedGraphEI<TEdgeInfoInput>& g, TFunction& f) {
  DirectedGraphEI<TEdgeInfoOutput> gnew(g.Size());
  for (unsigned u = 0; u < g.Size(); ++u) {
    for (const auto& e : g.EdgesEI(u)) {
      gnew.AddEdge(u, e.to, f(u, e.to, e.info));
    }
  }
  return gnew;
}
