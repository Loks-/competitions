#pragma once

#include "common/data_structures/disjoint_set.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace graph {
namespace mst {
// Kruskal's algorithm
// https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
// Time: O(V + E log E)
template <class TGraph, class TEdgeCostFunction>
inline std::pair<unsigned, typename TEdgeCostFunction::TEdgeCost> Kruskal(
    const TGraph& graph, const TEdgeCostFunction& f) {
  using TEdgeCost = typename TEdgeCostFunction::TEdgeCost;
  struct Edge {
    unsigned from;
    unsigned to;
    TEdgeCost cost;

    bool operator<(const Edge& r) const { return cost < r.cost; }
  };

  std::vector<Edge> edges;
  for (unsigned u = 0; u < graph.Size(); ++u) {
    for (auto e : graph.EdgesEI(u)) {
      if (u < e.to) edges.push_back({u, e.to, f(e.info)});
    }
  }
  std::sort(edges.begin(), edges.end());
  ds::DisjointSet ds(graph.Size());
  unsigned edges_added = 0;
  TEdgeCost total_cost = TEdgeCost();
  for (const auto& e : edges) {
    unsigned u1 = ds.Find(e.from);
    unsigned u2 = ds.Find(e.to);
    if (u1 != u2) {
      ++edges_added;
      total_cost += e.cost;
      ds.Union(u1, u2);
    }
  }
  return {edges_added, total_cost};
}
}  // namespace mst
}  // namespace graph
