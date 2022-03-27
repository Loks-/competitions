#pragma once

#include "common/graph/graph.h"

#include <algorithm>
#include <functional>
#include <stack>
#include <vector>

namespace graph {
namespace scc {
// Time: O(V + E)
inline std::vector<unsigned> PathBased(const DirectedGraph& graph) {
  unsigned n = graph.Size(), k = 0, l = 0;
  std::vector<unsigned> index(n, n), components(n, n);
  std::stack<unsigned> s, p;

  std::function<void(unsigned)> DFS = [&](unsigned u) -> void {
    index[u] = k++;
    s.push(u);
    p.push(u);
    for (auto v : graph.Edges(u)) {
      if (index[v] == n) {
        DFS(v);
      } else if (components[v] == n) {
        for (; index[p.top()] > index[v];) p.pop();
      }
    }
    if (p.top() == u) {
      for (;;) {
        auto v = s.top();
        s.pop();
        components[v] = l;
        if (v == u) break;
      }
      ++l;
      p.pop();
    }
  };

  for (unsigned u = 0; u < n; ++u) {
    if (index[u] == n) DFS(u);
  }
  return components;
}
}  // namespace scc
}  // namespace graph
