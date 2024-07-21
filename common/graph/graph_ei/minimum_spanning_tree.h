#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/mst/prim.h"

#include <utility>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::pair<unsigned, typename TEdgeCostFunction::TEdgeCost>
MinimumSpanningTree(const TGraph& g, const TEdgeCostFunction& f,
                    const TEdgeCost& max_cost) {
  return mst::Prim(g, f, max_cost);
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
inline std::pair<unsigned, TEdgeInfo> MinimumSpanningTree(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, TEdgeInfo max_cost) {
  return graph::MinimumSpanningTree(g, graph::EdgeCostProxy<TEdgeInfo>(),
                                    max_cost);
}
