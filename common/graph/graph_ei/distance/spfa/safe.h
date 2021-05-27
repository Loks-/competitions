#pragma once

#include "common/graph/cnone.h"

#include <queue>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Modification for SPFA to catch negative cycle.
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> Safe(const TGraph& graph,
                                   const TEdgeCostFunction& f, unsigned source,
                                   const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  std::vector<unsigned> l(gsize, 0);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::vector<unsigned> inq(gsize, 0);
  for (q.push(source); !q.empty();) {
    unsigned u = q.front();
    q.pop();
    inq[u] = 0;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        l[e.to] = l[u] + 1;
        if (l[e.to] >= gsize) return {};  // Negative cycle
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
