#pragma once

#include "common/graph/graph_ei.h"

#include <queue>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Pallottino algorithm
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> GoldfarbHaoKai(const TGraph& graph,
                                             const TEdgeCostFunction& f,
                                             unsigned source,
                                             const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::queue<unsigned> q0, q1;
  std::vector<unsigned> m(gsize, 0);
  q1.push(source);
  for (unsigned i = 0; !q1.empty(); ++i) {
    q0.swap(q1);
    for (; !q0.empty();) {
      unsigned u = q0.front();
      q0.pop();
      if (m[u] != i) continue;
      auto ucost = v[u];
      for (const auto& e : graph.EdgesEI(u)) {
        if (ucost + f(e.info) < v[e.to]) {
          v[e.to] = ucost + f(e.info);
          if (m[e.to] != i + 1) {
            q1.push(e.to);
            m[e.to] = i + 1;
          }
        }
      }
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
