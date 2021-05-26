#pragma once

#include "common/graph/cnone.h"

#include <queue>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Shortest Path Faster Algorithm + Skip if parent in the queue
// https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm
// Assume that no negative cycles exists
// Time: O(VE) worst case, O(E) on random graph
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> SIPITQ(const TGraph& graph,
                                     const TEdgeCostFunction& f,
                                     unsigned source,
                                     const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  std::vector<unsigned> p(gsize, CNone);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::vector<unsigned> inq(gsize, 0);
  p[source] = source;
  for (q.push(source); !q.empty();) {
    unsigned u = q.front();
    q.pop();
    if (inq[p[u]]) {
      // No reason to update u because parent of u will be updated again.
      inq[u] = 2;
      continue;
    }
    inq[u] = 0;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        p[e.to] = u;
        if (inq[e.to] != 1) {
          inq[e.to] = 1;
          q.push(e.to);
        }
      }
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
