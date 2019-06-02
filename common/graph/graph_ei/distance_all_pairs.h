#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include <algorithm>
#include <vector>

namespace graph {
// Floyd–Warshall algorithm.
// https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
// Time: O(V^3)
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<std::vector<TEdgeCost>> DistanceAllPairs(
    const TGraph& graph, const TEdgeCostFunction& f,
    const TEdgeCost& max_cost) {
  unsigned N = graph.Size();
  std::vector<std::vector<TEdgeCost>> vd(N,
                                         std::vector<TEdgeCost>(N, max_cost));
  for (unsigned u = 0; u < N; ++u) {
    for (auto e : graph.EdgesEI(u))
      vd[u][e.to] = std::min(vd[u][e.to], f(e.info));
    vd[u][u] = TEdgeCost();
  }
  for (unsigned k = 0; k < N; ++k) {
    for (unsigned i = 0; i < N; ++i) {
      if (!(vd[i][k] < max_cost)) continue;
      for (unsigned j = 0; j < N; ++j) {
        if ((vd[k][j] < max_cost) && (vd[i][k] + vd[k][j] < vd[i][j]))
          vd[i][j] = vd[i][k] + vd[k][j];
      }
    }
  }
  return vd;
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
std::vector<std::vector<TEdgeInfo>> DistanceAllPairs(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, TEdgeInfo max_cost) {
  return graph::DistanceAllPairs(g, graph::EdgeCostProxy<TEdgeInfo>(),
                                 max_cost);
}
