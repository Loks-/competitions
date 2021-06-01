#pragma once

#include "common/heap/monotone/base/bucket_queue.h"

#include <vector>

namespace graph {
namespace distance {
// Dial's algorithm with Monotone Bucket Heap.
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// All edges cost are not greater than max_edge_cost (C).
// Time: O(V + E + D), Memory O(E + C).
// D = O(EC) is max distance
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> Dial(const TGraph& graph,
                                   const TEdgeCostFunction& f, unsigned source,
                                   const TEdgeCost& max_cost,
                                   const TEdgeCost& max_edge_cost) {
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  std::vector<unsigned> visited(graph.Size(), 0);
  heap::monotone::base::BucketQueue<unsigned> q(max_edge_cost + 1);
  vd[source] = TEdgeCost();
  for (q.Add(TEdgeCost(), source); !q.Empty();) {
    auto p = q.Extract();
    unsigned u = p.value;
    if (!visited[u]) {
      visited[u] = true;
      for (auto e : graph.EdgesEI(u)) {
        unsigned v = e.to;
        TEdgeCost cost = p.priority + f(e.info);
        if (cost < vd[v]) {
          vd[v] = cost;
          q.Add(cost, v);
        }
      }
    }
  }
  return vd;
}
}  // namespace distance
}  // namespace graph
