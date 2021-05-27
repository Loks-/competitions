#pragma once

#include <deque>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Pape-Levit's algorithm
// For graphs without negative cycle.
// Time: O(V 2^V) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> Levit(const TGraph& graph,
                                    const TEdgeCostFunction& f, unsigned source,
                                    const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::deque<unsigned> q;
  std::vector<unsigned> m(gsize, 0);
  for (q.push_back(source); !q.empty();) {
    unsigned u = q.front();
    q.pop_front();
    m[u] = 2;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        if (m[e.to] == 0) {
          q.push_back(e.to);
        } else if (m[e.to] == 2) {
          q.push_front(e.to);
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
