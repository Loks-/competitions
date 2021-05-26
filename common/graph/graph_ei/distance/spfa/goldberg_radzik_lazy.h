#pragma once

#include <stack>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Goldberg-Radzik algorithm with lazy copy from b to a.
// For graphs without negative cycle. 
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> GoldbergRadzikLazy(const TGraph& graph,
                                                 const TEdgeCostFunction& f,
                                                 unsigned source,
                                                 const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::stack<unsigned> sa, sb;
  // 0 - untouched, 2 - in a, 3 - in b, 4 - in b + checked
  std::vector<unsigned> m(gsize, 0);
  sb.push(source);
  m[source] = 3;
  for (; !sb.empty();) {
    for (; !sb.empty();) {
      unsigned u = sb.top();
      if (m[u] == 3) {
        m[u] = 4;
        auto ucost = v[u];
        if (ucost < max_cost) {
          for (const auto& e : graph.EdgesEI(u)) {
            if (m[e.to] != 3) continue;
            if (ucost + f(e.info) <= v[e.to]) sb.push(e.to);
          }
        }
      } else {
        sb.pop();
        if (m[u] == 4) {
          m[u] = 2;
          sa.push(u);
        }
      }
    }
    for (; !sa.empty();) {
      unsigned u = sa.top();
      sa.pop();
      auto ucost = v[u];
      for (const auto& e : graph.EdgesEI(u)) {
        if (ucost + f(e.info) < v[e.to]) {
          v[e.to] = ucost + f(e.info);
          if (m[e.to] == 0) {
            sb.push(e.to);
            m[e.to] = 3;
          }
        }
      }
      m[u] = 0;
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
