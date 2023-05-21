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
  std::vector<unsigned> visited(n, 0), parent(n, graph::CNone),
      depth(n, graph::CNone), low(n, graph::CNone), art(n, 0);
  depth[0] = 0;
  std::stack<unsigned> s;
  for (s.push(0); !s.empty();) {
    auto u = s.top(), d = depth[u], p = parent[u];
    if (visited[u] == 0) {
      visited[u] = 1;
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
    } else if (visited[u] == 1) {
      visited[u] = 2;
      s.pop();
      if (art[u]) output.push_back(u);
      if (p == graph::CNone) continue;
      if (low[u] + 1 >= d) art[p] = 1;
      low[p] = std::min(low[p], low[u]);
    } else {
      s.pop();
    }
  }
  {
    unsigned u = 0;
    assert(output.back() == u);
    unsigned cc = 0;
    for (unsigned v : g.Edges(u)) {
      if (parent[v] == u) cc += 1;
    }
    if (cc < 2) output.pop_back();
  }
  return output;
}
}  // namespace ap
}  // namespace graph
