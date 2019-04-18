#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include <queue>
#include <utility>
#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  std::priority_queue<TPair, std::vector<TPair>, std::greater<TPair>> q;
  for (q.push({TEdgeCost(), source}); !q.empty(); q.pop()) {
    auto p = q.top();
    unsigned u = p.second;
    if (vd[u] == max_cost) {
      vd[u] = p.first;
      for (auto e : graph.EdgesEI(u)) {
        unsigned v = e.to;
        if (vd[v] == max_cost) {
          TEdgeCost cost = p.first + f(e.info);
          if (cost < max_cost) q.push({cost, v});
        }
      }
    }
  }
  return vd;
}
}  // namespace graph

template <class TEdgeInfo, bool directed_edges>
std::vector<TEdgeInfo> DistanceFromSourcePositiveCost(
    const graph::GraphEI<TEdgeInfo, directed_edges>& g, unsigned source,
    TEdgeInfo max_cost) {
  return graph::DistanceFromSourcePositiveCost(
      g, graph::EdgeCostProxy<TEdgeInfo>(), source, max_cost);
}
