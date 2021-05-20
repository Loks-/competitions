#pragma once

#include "common/graph/graph_ei.h"

#include <vector>

namespace graph {
namespace distance {
// Bellman-Ford algorithm with Yen modification.
// https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
// Time: O(VE)
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> BellmanFordYen(const TGraph& graph,
                                             const TEdgeCostFunction& f,
                                             unsigned source,
                                             const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  TGraph g1(gsize), g2(gsize);
  for (unsigned u = 0; u < gsize; ++u) {
    for (const auto& e : graph.EdgesEI(u))
      (u < e.to ? g1 : g2).AddBaseEdge(u, e.to, e.info);
  }
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  bool relaxed = true;
  for (unsigned i = 0; relaxed && (i < gsize + 2); i += 2) {
    relaxed = false;
    for (unsigned u = 0; u < gsize; ++u) {
      auto ucost = v[u];
      if (ucost == max_cost) continue;
      for (const auto& e : g1.EdgesEI(u)) {
        if (ucost + f(e.info) < v[e.to]) {
          v[e.to] = ucost + f(e.info);
          relaxed = true;
        }
      }
    }
    if (!relaxed && (i > 0)) break;
    relaxed = false;
    for (unsigned u = gsize; u-- > 0;) {
      auto ucost = v[u];
      if (ucost == max_cost) continue;
      for (const auto& e : g2.EdgesEI(u)) {
        if (ucost + f(e.info) < v[e.to]) {
          v[e.to] = ucost + f(e.info);
          relaxed = true;
        }
      }
    }
  }
  return relaxed ? std::vector<TEdgeCost>() : v;
}
}  // namespace distance
}  // namespace graph
