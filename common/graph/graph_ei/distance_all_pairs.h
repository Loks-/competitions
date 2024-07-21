#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/johnson.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<std::vector<TEdgeCost>> DistanceAllPairs(
    const TGraph& g, const TEdgeCostFunction& f, const TEdgeCost& max_cost) {
  return distance::Johnson(g, f, max_cost);
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
inline std::vector<std::vector<TEdgeInfo>> DistanceAllPairs(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, TEdgeInfo max_cost) {
  return graph::DistanceAllPairs(g, graph::EdgeCostProxy<TEdgeInfo>(),
                                 max_cost);
}
