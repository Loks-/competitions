#pragma once

#include "common/data_structures/disjoint_set.h"
#include "common/data_structures/unsigned_set.h"
#include "common/graph/graph.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace graph {
namespace mst {
// Boruvka's algorithm
// https://en.wikipedia.org/wiki/Bor%C5%AFvka%27s_algorithm
// Time: O(E log V)
template <class TGraph, class TEdgeCostFunction>
inline std::pair<unsigned, typename TEdgeCostFunction::TEdgeCost> Boruvka(
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
  ds::DisjointSet ds(graph.Size());
  ds::UnsignedSet edges_set(edges.size());
  ds::UnsignedSet vertexes_set(graph.Size());
  std::vector<unsigned> remove_list;
  std::vector<unsigned> best_edge(graph.Size(), CNone);

  edges_set.InsertAll();
  unsigned edges_added = 0;
  TEdgeCost total_cost = TEdgeCost();
  for (bool stop = false; !stop && (ds.GetSetsCount() > 1);) {
    vertexes_set.Clear();
    remove_list.clear();
    stop = true;
    for (auto ei : edges_set.List()) {
      const auto& e = edges[ei];
      unsigned u1 = ds.Find(e.from), u2 = ds.Find(e.to);
      if (u1 == u2) {
        remove_list.push_back(ei);
      } else {
        vertexes_set.Insert(u1);
        vertexes_set.Insert(u2);
        if ((best_edge[u1] == CNone) || (e < edges[best_edge[u1]]))
          best_edge[u1] = ei;
        if ((best_edge[u2] == CNone) || (e < edges[best_edge[u2]]))
          best_edge[u2] = ei;
      }
    }
    for (auto ei : remove_list) edges_set.Remove(ei);
    for (auto u : vertexes_set.List()) {
      auto ei = best_edge[u];
      best_edge[u] = CNone;
      if (!edges_set.HasKey(ei)) continue;
      const auto& e = edges[ei];
      ++edges_added;
      total_cost += e.cost;
      edges_set.Remove(ei);
      ds.Union(e.from, e.to);
      stop = false;
    }
  }
  return {edges_added, total_cost};
}
}  // namespace mst
}  // namespace graph
