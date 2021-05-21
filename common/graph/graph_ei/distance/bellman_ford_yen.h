#pragma once

#include "common/graph/graph_ei.h"

#include <algorithm>
#include <utility>
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
  std::vector<std::pair<unsigned, TEdgeCost>> v1, v2;
  std::vector<unsigned> v1b(gsize + 1), v2b(gsize + 1);
  for (unsigned u = 0; u < gsize; ++u) {
    v1b[u] = v1.size();
    v2b[u] = v2.size();
    for (const auto& e : graph.EdgesEI(u))
      (u < e.to ? v1 : v2).push_back({e.to, f(e.info)});
  }
  v1b[gsize] = v1.size();
  v2b[gsize] = v2.size();
  for (auto& k : v2b) k = v2.size() - k;
  std::reverse(v2.begin(), v2.end());
  std::reverse(v2b.begin(), v2b.end());

  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  bool relaxed = true;
  for (unsigned i = 0; relaxed && (i < gsize + 2); i += 2) {
    relaxed = false;
    for (unsigned u = 0; u < gsize; ++u) {
      auto ucost = v[u];
      if (ucost == max_cost) continue;
      for (unsigned i = v1b[u], iend = v1b[u + 1]; i < iend; ++i) {
        const auto& p = v1[i];
        if (ucost + p.second < v[p.first]) {
          v[p.first] = ucost + p.second;
          relaxed = true;
        }
      }
    }
    if (!relaxed && (i > 0)) break;
    relaxed = false;
    for (unsigned u = 0; u < gsize; ++u) {
      auto ucost = v[gsize - 1 - u];
      if (ucost == max_cost) continue;
      for (unsigned i = v2b[u], iend = v2b[u + 1]; i < iend; ++i) {
        const auto& p = v2[i];
        if (ucost + p.second < v[p.first]) {
          v[p.first] = ucost + p.second;
          relaxed = true;
        }
      }
    }
  }
  return relaxed ? std::vector<TEdgeCost>() : v;
}
}  // namespace distance
}  // namespace graph
