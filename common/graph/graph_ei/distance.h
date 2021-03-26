#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/bellman_ford.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> DistanceFromSource(const TGraph& graph,
                                                 const TEdgeCostFunction& f,
                                                 unsigned source,
                                                 const TEdgeCost& max_cost) {
  return distance::BellmanFord(graph, f, source, max_cost);
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
inline std::vector<TEdgeInfo> DistanceFromSource(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, unsigned source,
    TEdgeInfo max_cost) {
  return graph::DistanceFromSource(g, graph::EdgeCostProxy<TEdgeInfo>(), source,
                                   max_cost);
}
