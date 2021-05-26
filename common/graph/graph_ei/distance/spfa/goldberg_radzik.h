#pragma once

#include <stack>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Goldberg-Radzik algorithm
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> GoldbergRadzik(const TGraph& graph,
                                             const TEdgeCostFunction& f,
                                             unsigned source,
                                             const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::stack<unsigned> sa, sb;
  // 0 - untouched, 2 - in a, 3 - in b, 4 - recursive call, 5 - processing
  std::vector<unsigned> m(gsize, 0);
  sb.push(source);
  m[source] = 3;
  for (; !sb.empty();) {
    for (; !sb.empty();) {
      unsigned u = sb.top();
      auto ucost = v[u];
      switch (m[u]) {
        case 0:
        case 2:
          sb.pop();
          break;
        case 3:
          m[u] = 0;
          for (const auto& e : graph.EdgesEI(u)) {
            if (ucost + f(e.info) < v[e.to]) {
              m[u] = 5;                    // Processing
              if (m[e.to] == 2) continue;  // Already in a.
              m[e.to] = 4;
              sb.push(e.to);
            }
          }
          break;
        case 4:
          m[u] = 5;
          if (ucost < max_cost) {
            for (const auto& e : graph.EdgesEI(u)) {
              if (m[e.to] == 2) continue;  // Already in a.
              if (ucost + f(e.info) <= v[e.to]) {
                if (m[e.to] == 5) return {};  // Negative cycle
                m[e.to] = 4;
                sb.push(e.to);
              }
            }
          }
          break;
        case 5:
          sb.pop();
          sa.push(u);
          m[u] = 2;
          break;
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
