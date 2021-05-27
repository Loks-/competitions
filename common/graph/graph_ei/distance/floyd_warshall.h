#pragma once

#include <algorithm>
#include <vector>

namespace graph {
namespace distance {
// Floyd–Warshall algorithm.
// https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
// Time: O(V^3)
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<std::vector<TEdgeCost>> FloydWarshall(
    const TGraph& graph, const TEdgeCostFunction& f,
    const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<std::vector<TEdgeCost>> vd(gsize,
                                         std::vector<TEdgeCost>(gsize, max_cost));
  for (unsigned u = 0; u < gsize; ++u) {
    for (auto e : graph.EdgesEI(u))
      vd[u][e.to] = std::min(vd[u][e.to], f(e.info));
    vd[u][u] = TEdgeCost();
  }
  for (unsigned k = 0; k < gsize; ++k) {
    for (unsigned i = 0; i < gsize; ++i) {
      if (!(vd[i][k] < max_cost)) continue;
      for (unsigned j = 0; j < gsize; ++j) {
        if ((vd[k][j] < max_cost) && (vd[i][k] + vd[k][j] < vd[i][j]))
          vd[i][j] = vd[i][k] + vd[k][j];
      }
    }
  }
  return vd;
}
}  // namespace distance
}  // namespace graph
