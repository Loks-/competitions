#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/heap/ukey_value_map.h"

#include <functional>
#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  heap::UKeyValueMap<TEdgeCost> q(
      std::vector<TEdgeCost>(graph.Size(), max_cost), true);
  for (q.Set(source, TEdgeCost()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    TEdgeCost ucost = q.Get(u);
    for (auto e : graph.EdgesEI(u)) {
      unsigned v = e.to;
      TEdgeCost cost = ucost + f(e.info);
      if (cost < q.Get(v)) {
        q.Set(v, cost);
      }
    }
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
