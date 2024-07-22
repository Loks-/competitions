#pragma once

#include "common/base.h"
#include "common/graph/graph.h"
#include "common/heap/ukvm/dheap.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace graph {
namespace scycle {
// Find shortest cycle length by vertex exclusion.
// Works for sparse graphs.
namespace hidden {
unsigned VertexExclusionI(
    std::vector<std::unordered_map<unsigned, unsigned>>& edges) {
  unsigned best_cost = unsigned(-1);
  std::vector<unsigned> vpc;
  vpc.reserve(edges.size());
  for (auto& m : edges) vpc.push_back(m.size());
  heap::ukvm::DHeap<4, unsigned> h(vpc, false);
  for (; !h.Empty();) {
    auto u = h.ExtractKey();
    for (auto it1 = edges[u].begin(); it1 != edges[u].end(); ++it1) {
      if (it1->second + 2 >= best_cost) continue;
      for (auto it2 = edges[u].begin(); it2 != it1; ++it2) {
        auto u1 = it1->first, u2 = it2->first;
        auto d12 = it1->second + it2->second;
        if (d12 + 1 >= best_cost) continue;
        auto itu12 = edges[u1].find(u2);
        if (itu12 == edges[u1].end()) {
          edges[u1][u2] = edges[u2][u1] = d12;
        } else {
          best_cost = std::min(best_cost, itu12->second + d12);
          if (itu12->second > d12) {
            edges[u1][u2] = edges[u2][u1] = d12;
          }
        }
      }
    }
    for (auto it1 : edges[u]) {
      auto u1 = it1.first;
      edges[u1].erase(u);
      h.Set(u1, edges[u1].size());
    }
  }
  return best_cost;
}
}  // namespace hidden

unsigned VertexExclusion(const UndirectedGraph& g) {
  // Check length 1
  for (unsigned u = 0; u < g.Size(); ++u) {
    for (auto& v : g.Edges(u)) {
      if (v == u) return 1;
    }
  }
  std::vector<std::unordered_map<unsigned, unsigned>> edges(g.Size());
  for (unsigned u = 0; u < g.Size(); ++u) {
    for (auto& v : g.Edges(u)) {
      if (edges[u].find(v) != edges[u].end()) return 2;
      edges[u][v] = 1u;
    }
  }
  return hidden::VertexExclusionI(edges);
}
}  // namespace scycle
}  // namespace graph
