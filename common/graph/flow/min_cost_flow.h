#pragma once

#include "common/graph/flow/edge_cost_proxy.h"
#include "common/graph/flow/graph.h"
#include "common/graph/flow/min_cost/edmonds_karp_spfa.h"

template <class TEdge>
inline typename TEdge::TData MinCostFlow(
    graph::flow::Graph<TEdge>& g, const typename TEdge::TFlow& max_flow) {
  return graph::flow::min_cost::EdmondsKarpSPFA(
      g, max_flow, graph::flow::EdgeCostProxy<TEdge>());
}
