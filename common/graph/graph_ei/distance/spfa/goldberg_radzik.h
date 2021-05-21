#pragma once

#include "common/graph/graph_ei.h"

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
  std::stack<unsigned> sa, sb, st;
  // 0 - untouched, 1 - scaned, 2 - in a, 3 - in b, 4 - in b + checked
  std::vector<unsigned> m(gsize, 0);
  sb.push(source);
  m[source] = 3;
  for (unsigned i = 0; !sb.empty(); ++i) {
    for (; !sb.empty();) {
      unsigned u = sb.top();
      if (m[u] == 3) {
        m[u] = 4;
        auto ucost = v[u];
        for (const auto& e : graph.EdgesEI(u)) {
          if (m[e.to] != 3) continue;
          if (ucost + f(e.info) < v[e.to]) sb.push(e.to);
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
          sb.push(e.to);
          m[e.to] = 3;
        }
      }
      m[u] = 1;
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
