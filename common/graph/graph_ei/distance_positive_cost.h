#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/dijkstra.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> DistanceFromSourcePositiveCost(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  return distance::Dijkstra(graph, f, source, max_cost);
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
inline std::vector<TEdgeInfo> DistanceFromSourcePositiveCost(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, unsigned source,
    TEdgeInfo max_cost) {
  return graph::DistanceFromSourcePositiveCost(
      g, graph::EdgeCostProxy<TEdgeInfo>(), source, max_cost);
}
