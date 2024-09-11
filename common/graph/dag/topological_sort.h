#pragma once

#include "common/graph/graph.h"

#include <queue>
#include <vector>

inline std::vector<unsigned> TopologicalSort(const DirectedGraph& g) {
  std::vector<unsigned> ein(g.Size()), order;
  std::queue<unsigned> q;
  for (unsigned u = 0; u < g.Size(); ++u) {
    ein[u] = g.InvertedEdges(u).size();
    if (ein[u] == 0) q.push(u);
  }
  for (; !q.empty(); q.pop()) {
    const auto u = q.front();
    order.push_back(u);
    for (auto v : g.Edges(u)) {
      if (--ein[v] == 0) q.push(v);
    }
  }
  return order;
}
