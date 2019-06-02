#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/floyd_warshall.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<std::vector<TEdgeCost>> DistanceAllPairs(
    const TGraph& graph, const TEdgeCostFunction& f,
    const TEdgeCost& max_cost) {
  return distance::FloydWarshall(graph, f, max_cost);
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
std::vector<std::vector<TEdgeInfo>> DistanceAllPairs(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, TEdgeInfo max_cost) {
  return graph::DistanceAllPairs(g, graph::EdgeCostProxy<TEdgeInfo>(),
                                 max_cost);
}
