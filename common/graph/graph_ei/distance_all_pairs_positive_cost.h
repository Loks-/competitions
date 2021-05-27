#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/dijkstra.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<std::vector<TEdgeCost>> DistanceAllPairsPositiveCost(
    const TGraph& graph, const TEdgeCostFunction& f,
    const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<std::vector<TEdgeCost>> vd;
  vd.reserve(gsize);
  for (unsigned i = 0; i < gsize; ++i)
    vd.emplace_back(distance::Dijkstra(graph, f, i, max_cost));
  return vd;
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
inline std::vector<std::vector<TEdgeInfo>> DistanceAllPairsPositiveCost(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, TEdgeInfo max_cost) {
  return graph::DistanceAllPairsPositiveCost(
      g, graph::EdgeCostProxy<TEdgeInfo>(), max_cost);
}
