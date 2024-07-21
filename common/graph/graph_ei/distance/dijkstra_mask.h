#pragma once

#include "common/data_structures/unsigned_set.h"
#include "common/heap.h"

#include <vector>

namespace graph {
namespace distance {
// Dijkstra's algorithm with d-ary heap.
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// All edges cost are non-negative.
// Only subgraph (based on mask) is used for calculations
// Time: O((V + E) log V)
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline void DijkstraMask(const TGraph& g, const TEdgeCostFunction& f,
                         unsigned source, const TEdgeCost& max_cost,
                         const ds::UnsignedSet& mask,
                         std::vector<TEdgeCost>& output) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  using THeap = HeapMinOnTop<TPair>;
  thread_local std::vector<unsigned> visited;
  if (visited.size() < g.Size()) visited.resize(g.Size());
  for (auto u : mask.List()) {
    output[u] = max_cost;
    visited[u] = false;
  }
  if (!mask.HasKey(source)) return;
  THeap q;
  output[source] = TEdgeCost();
  for (q.Add({TEdgeCost(), source}); !q.Empty();) {
    auto p = q.Extract();
    unsigned u = p.second;
    if (!visited[u]) {
      visited[u] = true;
      for (auto e : g.EdgesEI(u)) {
        unsigned v = e.to;
        if (!mask.HasKey(v)) continue;
        TEdgeCost cost = p.first + f(e.info);
        if (cost < output[v]) {
          output[v] = cost;
          q.Add({cost, v});
        }
      }
    }
  }
}
}  // namespace distance
}  // namespace graph
