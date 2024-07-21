#pragma once

#include "common/graph/edge.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/dijkstra.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/vector/min.h"

#include <algorithm>
#include <vector>

namespace graph {
namespace distance {
// Johnson algorithm.
// https://en.wikipedia.org/wiki/Johnson%27s_algorithm
// Time: O(V (V + E) log(V))
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<std::vector<TEdgeCost>> Johnson(const TGraph& g,
                                                   const TEdgeCostFunction& f,
                                                   const TEdgeCost& max_cost) {
  unsigned gsize = g.Size();
  // Bellman-Ford
  std::vector<TEdgeCost> vh(gsize, TEdgeCost());
  bool relaxed = true;
  for (unsigned i = 0; relaxed && (i < gsize); ++i) {
    relaxed = false;
    for (unsigned u = 0; u < gsize; ++u) {
      auto ucost = vh[u];
      for (const auto& e : g.EdgesEI(u)) {
        if (ucost + f(e.info) < vh[e.to]) {
          vh[e.to] = ucost + f(e.info);
          relaxed = true;
        }
      }
    }
  }
  if (relaxed) return {};  // Negative cycle
  // New graph
  TEdgeCost adjust = nvector::Min(vh);
  GraphEI<TEdgeCost, true> g2(gsize);
  for (unsigned u = 0; u < gsize; ++u) {
    for (const auto& e : g.EdgesEI(u))
      g2.AddBaseEdge(u, e.to, f(e.info) + vh[u] - vh[e.to]);
  }
  EdgeCostProxy<TEdgeCost> fp;
  std::vector<std::vector<TEdgeCost>> vd(gsize);
  for (unsigned u = 0; u < gsize; ++u) {
    auto vu = distance::Dijkstra(g2, fp, u, max_cost - adjust);
    for (unsigned v = 0; v < gsize; ++v)
      vu[v] = std::min(vu[v] + vh[v] - vh[u], max_cost);
    vd[u] = vu;
  }
  return vd;
}
}  // namespace distance
}  // namespace graph
