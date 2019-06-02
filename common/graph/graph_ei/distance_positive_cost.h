#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/heap/dheap_ukey_value_map.h"
#include <functional>
#include <vector>

namespace graph {
// Dijkstra's algorithm with d-ary heap.
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// Time: O((V + E) log V)
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  heap::DHeapUKeyValueMap<4u, TEdgeCost> q(
      std::vector<TEdgeCost>(graph.Size(), max_cost), true);
  for (q.AddNewKey(source, TEdgeCost()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    TEdgeCost ucost = q.Get(u);
    for (auto e : graph.EdgesEI(u))
      q.DecreaseValueIfLess(e.to, ucost + f(e.info));
  }
  return q.GetValues();
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
std::vector<TEdgeInfo> DistanceFromSourcePositiveCost(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, unsigned source,
    TEdgeInfo max_cost) {
  return graph::DistanceFromSourcePositiveCost(
      g, graph::EdgeCostProxy<TEdgeInfo>(), source, max_cost);
}
