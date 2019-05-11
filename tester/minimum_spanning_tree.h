#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/heap/binary_heap.h"

#include <algorithm>
#include <utility>
#include <vector>

template <class TGraph, class TEdgeCostFunction>
std::pair<unsigned, typename TEdgeCostFunction::TEdgeCost>
MinimumSpanningTree_Prim_BH(const TGraph& graph, const TEdgeCostFunction& f,
                            unsigned source = 0) {
  using TEdgeCost = typename TEdgeCostFunction::TEdgeCost;
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<unsigned> visited(graph.Size(), 0);
  unsigned edges_added = -1u;
  TEdgeCost total_cost = TEdgeCost();
  heap::BinaryHeap<TPair> q;
  for (q.Add({TEdgeCost(), source}); !q.Empty();) {
    auto p = q.Extract();
    if (!visited[p.second]) {
      visited[p.second] = true;
      ++edges_added;
      total_cost += p.first;
      for (auto e : graph.EdgesEI(p.second)) {
        if (!visited[e.to]) {
          q.Add({f(e.info), e.to});
        }
      }
    }
  }
  return {edges_added, total_cost};
}

template <class TGraph, class TEdgeCostFunction>
std::pair<unsigned, typename TEdgeCostFunction::TEdgeCost>
MinimumSpanningTree_Prim_CBH(const TGraph& graph, const TEdgeCostFunction& f,
                             typename TEdgeCostFunction::TEdgeCost max_cost,
                             unsigned source = 0) {
  using TEdgeCost = typename TEdgeCostFunction::TEdgeCost;
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<unsigned> visited(graph.Size(), 0);
  std::vector<unsigned> best_cost_in_queue(graph.Size(), max_cost);
  unsigned edges_added = -1u;
  TEdgeCost total_cost = TEdgeCost();
  heap::BinaryHeap<TPair> q;
  for (q.Add({TEdgeCost(), source}); !q.Empty();) {
    auto p = q.Extract();
    if (!visited[p.second]) {
      visited[p.second] = true;
      ++edges_added;
      total_cost += p.first;
      for (auto e : graph.EdgesEI(p.second)) {
        TEdgeCost cost = f(e.info);
        if (!visited[e.to] && cost < best_cost_in_queue[e.to]) {
          best_cost_in_queue[e.to] = cost;
          q.Add({cost, e.to});
        }
      }
    }
  }
  return {edges_added, total_cost};
}
