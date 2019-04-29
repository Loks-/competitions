#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/heap/binary_heap.h"
#include <queue>
#include <utility>
#include <vector>

namespace graph {
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_PQ(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  std::priority_queue<TPair, std::vector<TPair>, std::greater<TPair>> q;
  for (q.push({TEdgeCost(), source}); !q.empty();) {
    auto p = q.top();
    q.pop();
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

template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_BH(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  heap::BinaryHeap<TPair, std::greater<TPair>> q;
  for (q.Add({TEdgeCost(), source}); !q.Empty();) {
    auto p = q.Extract();
    unsigned u = p.second;
    if (vd[u] == max_cost) {
      vd[u] = p.first;
      for (auto e : graph.EdgesEI(u)) {
        unsigned v = e.to;
        if (vd[v] == max_cost) {
          TEdgeCost cost = p.first + f(e.info);
          if (cost < max_cost) q.Add({cost, v});
        }
      }
    }
  }
  return vd;
}
}  // namespace graph
