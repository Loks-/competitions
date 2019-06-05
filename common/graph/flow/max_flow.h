#pragma once

#include "common/graph/flow/graph.h"
#include "common/graph/flow/max_flow/edmonds_karp.h"

template <class TEdge>
inline typename TEdge::TFlow MaxFlow(graph::flow::Graph<TEdge>& g) {
  return graph::flow::MaxFlowEdmondsKarp(g);
}
