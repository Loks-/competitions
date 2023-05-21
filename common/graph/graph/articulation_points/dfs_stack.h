#pragma once

#include "common/graph/graph.h"

#include <algorithm>
#include <stack>
#include <vector>

namespace graph {
namespace ap {
// Time: O(V + E)
inline std::vector<unsigned> DFSStack(const UndirectedGraph& g) {
  std::vector<unsigned> output;
  unsigned n = g.Size();
  if (n <= 1) return output;
  std::vector<unsigned> visited(n, 0), parent(n, graph::CNone), depth(n, 0),
      low(n, graph::CNone), art(n, 0);
  std::stack<unsigned> s;
  for (unsigned u = 0; u < n; ++u) s.push(u);
  for (; !s.empty();) {
    auto u = s.top(), d = depth[u], p = parent[u], vu = visited[u]++;
    if (vu) s.pop();
    if (vu == 0) {
      low[u] = d;
      for (auto v : g.Edges(u)) {
        if (!visited[v]) {
          parent[v] = u;
          depth[v] = d + 1;
          s.push(v);
        } else if (v != p) {
          low[u] = std::min(low[u], depth[v]);
        }
      }
    } else if (vu == 1) {
      if (p == graph::CNone) {
        unsigned cc = 0;
        for (unsigned v : g.Edges(u)) {
          if (parent[v] == u) ++cc;
        }
        if (cc > 1) output.push_back(u);
      } else {
        if (art[u]) output.push_back(u);
        if (low[u] + 1 >= d) art[p] = 1;
        low[p] = std::min(low[p], low[u]);
      }
      if (p == graph::CNone) continue;
    }
  }
  return output;
}
}  // namespace ap
}  // namespace graph
