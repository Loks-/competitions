#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/spfa/spfa.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> DistanceFromSourceSkipCycleCheck(
    const TGraph& g, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  return distance::spfa::SPFA(g, f, source, max_cost);
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
inline std::vector<TEdgeInfo> DistanceFromSourceSkipCycleCheck(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, unsigned source,
    TEdgeInfo max_cost) {
  return graph::DistanceFromSourceSkipCycleCheck(
      g, graph::EdgeCostProxy<TEdgeInfo>(), source, max_cost);
}
