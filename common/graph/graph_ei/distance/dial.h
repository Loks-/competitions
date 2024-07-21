#pragma once

#include "common/heap/monotone/ukvm/rolling_bucket_queue_dll.h"

#include <vector>

namespace graph {
namespace distance {
// Dial's algorithm with Monotone Bucket Heap.
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// All edges cost are not greater than max_edge_cost (C).
// Time: O(V + E + D), Memory O(V + C).
// D = O(EC) is max distance
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> Dial(const TGraph& g, const TEdgeCostFunction& f,
                                   unsigned source, const TEdgeCost& max_cost,
                                   const TEdgeCost& max_edge_cost) {
  heap::monotone::ukvm::RollingBucketQueueDLL q(
      std::vector<TEdgeCost>(g.Size(), max_cost), true, max_edge_cost + 1);
  for (q.AddNewKey(source, TEdgeCost()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    TEdgeCost ucost = q.Get(u);
    for (auto e : g.EdgesEI(u)) q.DecreaseValueIfLess(e.to, ucost + f(e.info));
  }
  return q.GetValues();
}
}  // namespace distance
}  // namespace graph
