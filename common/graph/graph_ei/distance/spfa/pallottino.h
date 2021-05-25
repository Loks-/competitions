#pragma once

#include <queue>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Pallottino algorithm
// Time: O(V^2 E) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> Pallottino(const TGraph& graph,
                                         const TEdgeCostFunction& f,
                                         unsigned source,
                                         const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size(), u;
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::queue<unsigned> q1, q2;
  std::vector<unsigned> m(gsize, 0);
  for (q2.push(source); !q2.empty() || !q1.empty();) {
    if (q1.empty()) {
      u = q2.front();
      q2.pop();
    } else {
      u = q1.front();
      q1.pop();
    }
    m[u] = 2;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        if (m[e.to] == 0) {
          q2.push(e.to);
        } else if (m[e.to] == 2) {
          q1.push(e.to);
        }
        m[e.to] = 1;
      }
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
