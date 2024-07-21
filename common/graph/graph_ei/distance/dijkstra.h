#pragma once

#include "common/heap/ukvm/dheap.h"

#include <vector>

namespace graph {
namespace distance {
// Dijkstra's algorithm with d-ary heap.
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// All edges cost are non-negative.
// Time: O((V + E) log V)
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> Dijkstra(const TGraph& g,
                                       const TEdgeCostFunction& f,
                                       unsigned source,
                                       const TEdgeCost& max_cost) {
  heap::ukvm::DHeap<4u, TEdgeCost> q(std::vector<TEdgeCost>(g.Size(), max_cost),
                                     true);
  for (q.AddNewKey(source, TEdgeCost()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    TEdgeCost ucost = q.Get(u);
    for (auto e : g.EdgesEI(u)) q.DecreaseValueIfLess(e.to, ucost + f(e.info));
  }
  return q.GetValues();
}
}  // namespace distance
}  // namespace graph
