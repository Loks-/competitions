#pragma once

#include "common/graph/graph.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace graph {
namespace ap {
// Time: O(V + E)
inline std::vector<unsigned> DFSRecursion(const UndirectedGraph& g) {
  std::vector<unsigned> output;
  const unsigned n = g.Size();
  if (n <= 1) return output;
  std::vector<unsigned> visited(n, 0), parent(n, graph::CNone),
      depth(n, graph::CNone), low(n, graph::CNone);

  const std::function<void(unsigned, unsigned)> DFS = [&](unsigned u,
                                                          unsigned d) -> void {
    visited[u] = 1;
    depth[u] = d;
    low[u] = d;
    unsigned cc = 0;
    bool ap = false;
    for (auto v : g.Edges(u)) {
      if (!visited[v]) {
        parent[v] = u;
        DFS(v, d + 1);
        cc += 1;
        if (low[v] >= depth[u]) ap = true;
        low[u] = std::min(low[u], low[v]);
      } else if (v != parent[u]) {
        low[u] = std::min(low[u], depth[v]);
      }
    }
    if (((parent[u] == graph::CNone) && (cc > 1)) ||
        ((parent[u] != graph::CNone) && ap))
      output.push_back(u);
  };

  for (unsigned u = 0; u < n; ++u) {
    if (!visited[u]) DFS(u, 0);
  }
  return output;
}
}  // namespace ap
}  // namespace graph
