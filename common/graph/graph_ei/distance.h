#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include <vector>

namespace graph {
// Bellman-Ford algorithm.
// https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
// Time: O(VE)
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSource(const TGraph& graph,
                                          const TEdgeCostFunction& f,
                                          unsigned source,
                                          const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  bool relaxed = true;
  for (unsigned i = 0; relaxed && (i < gsize); ++i) {
    relaxed = false;
    for (unsigned u = 0; u < gsize; ++u) {
      auto ucost = v[u];
      if (ucost == max_cost) continue;
      for (const auto& e : graph.EdgesEI(u)) {
        if (ucost + f(e.info) < v[e.to]) {
          v[e.to] = ucost + f(e.info);
          relaxed = true;
        }
      }
    }
  }
  return relaxed ? std::vector<TEdgeCost>() : v;
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
std::vector<TEdgeInfo> DistanceFromSourcet(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, unsigned source,
    TEdgeInfo max_cost) {
  return graph::DistanceFromSource(g, graph::EdgeCostProxy<TEdgeInfo>(), source,
                                   max_cost);
}
