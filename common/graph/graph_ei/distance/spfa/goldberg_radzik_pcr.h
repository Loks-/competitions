#pragma once

#include "common/graph/tree/pcr.h"

#include <stack>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Goldberg-Radzik algorithm
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> GoldbergRadzikPCR(const TGraph& graph,
                                                const TEdgeCostFunction& f,
                                                unsigned source,
                                                const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::stack<unsigned> sa, sb;
  // 0 - untouched, 2 - in a, 3 - in b, 4 - processing
  std::vector<unsigned> m(gsize, 0);
  PCR pcr(gsize);
  sb.push(source);
  m[source] = 3;
  for (; !sb.empty();) {
    for (; !sb.empty();) {
      unsigned u = sb.top();
      switch (m[u]) {
        case 0:
        case 2:
          sb.pop();
          break;
        case 3: {
          m[u] = 4;
          const auto& upcr = pcr(u);
          for (auto c : upcr.childs) {
            if (m[c] == 2) continue;   // Already in a.
            if (m[c] == 4) return {};  // Negative cycle
            m[c] = 3;
            sb.push(c);
          }
        } break;
        case 4:
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
          pcr.SetP(e.to, u);
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
