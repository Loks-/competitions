#pragma once

#include <queue>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Shortest Path Faster Algorithm.
// https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm
// For graphs without negative cycle.
// Time: O(VE) worst case, O(E) on random graph
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> SPFA(const TGraph& g, const TEdgeCostFunction& f,
                                   unsigned source, const TEdgeCost& max_cost) {
  unsigned gsize = g.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::vector<unsigned> inq(gsize, 0);
  for (q.push(source); !q.empty();) {
    unsigned u = q.front();
    q.pop();
    inq[u] = 0;
    auto ucost = v[u];
    for (const auto& e : g.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        if (!inq[e.to]) {
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
