#pragma once

#include "common/graph/flow/edge_cost_proxy.h"
#include "common/graph/flow/graph.h"
// #include "common/graph/flow/min_cost/basaker_gowen_spfa.h"
#include "common/graph/flow/min_cost/basaker_gowen_dijkstra.h"
#include "common/heap/ukvm/dheap.h"

template <class TEdge>
inline typename TEdge::TData MinCostFlow(
    graph::flow::Graph<TEdge>& g, const typename TEdge::TFlow& max_flow) {
  // return graph::flow::min_cost::BasakerGowenSPFA(
  //     g, max_flow, graph::flow::EdgeCostProxy<TEdge>());
  heap::ukvm::DHeap<4, typename TEdge::TData> q(g.Size());
  return graph::flow::min_cost::BasakerGowenDijkstra(
      g, max_flow, graph::flow::EdgeCostProxy<TEdge>(), q);
}
