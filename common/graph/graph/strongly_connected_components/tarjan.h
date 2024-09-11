#pragma once

#include "common/graph/graph.h"

#include <algorithm>
#include <functional>
#include <stack>
#include <vector>

namespace graph {
namespace scc {
// Time: O(V + E)
// Components are in the reverse topological sort order.
inline std::vector<unsigned> Tarjan(const DirectedGraph& g) {
  const unsigned n = g.Size();
  unsigned k = 0, l = 0;
  std::vector<unsigned> index(n, n), lowlink(n, 0), instack(n, 0),
      components(n);
  std::stack<unsigned> s;

  const std::function<void(unsigned)> SConnect = [&](unsigned u) -> void {
    index[u] = lowlink[u] = k++;
    s.push(u);
    instack[u] = 1;
    for (auto v : g.Edges(u)) {
      if (index[v] == n) {
        SConnect(v);
        lowlink[u] = std::min(lowlink[u], lowlink[v]);
      } else if (instack[v]) {
        lowlink[u] = std::min(lowlink[u], index[v]);
      }
    }
    if (lowlink[u] == index[u]) {
      for (;;) {
        const auto v = s.top();
        s.pop();
        instack[v] = 0;
        components[v] = l;
        if (v == u) break;
      }
      ++l;
    }
  };

  for (unsigned u = 0; u < n; ++u) {
    if (index[u] == n) SConnect(u);
  }
  return components;
}
}  // namespace scc
}  // namespace graph
