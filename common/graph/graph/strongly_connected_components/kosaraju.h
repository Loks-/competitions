#pragma once

#include "common/graph/graph.h"

#include <algorithm>
#include <functional>
#include <stack>
#include <vector>

namespace graph {
namespace scc {
// Time: O(V + E)
inline std::vector<unsigned> Kosaraju(const DirectedGraph& g) {
  const unsigned n = g.Size();
  unsigned l = 0;
  std::vector<unsigned> visited(n, 0), vl, components(n, n);
  vl.reserve(n);

  const std::function<void(unsigned)> Visit = [&](unsigned u) -> void {
    if (visited[u]) return;
    visited[u] = 1;
    for (auto v : g.Edges(u)) Visit(v);
    vl.push_back(u);
  };

  const std::function<void(unsigned, unsigned)> Assign =
      [&](unsigned u, unsigned index) -> void {
    if (components[u] < n) return;
    components[u] = index;
    for (auto v : g.InvertedEdges(u)) Assign(v, index);
  };

  for (unsigned u = 0; u < n; ++u) Visit(u);
  std::reverse(vl.begin(), vl.end());
  for (auto u : vl) {
    if (components[u] == n) Assign(u, l++);
  }
  return components;
}
}  // namespace scc
}  // namespace graph
