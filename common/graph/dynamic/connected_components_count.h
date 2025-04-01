#pragma once

#include "common/graph/dynamic/graph.h"

#include <stack>
#include <vector>

namespace graph {
namespace dynamic {
// Time: O(V + E)
template <class TData>
inline unsigned ConnectedComponentsCount(const UndirectedGraph<TData>& g) {
  const unsigned n = g.Size();
  unsigned r = 0;
  std::vector<unsigned> visited(n, 0);
  std::stack<unsigned> s;
  for (unsigned i = 0; i < n; ++i) {
    if (visited[i] == 0) {
      visited[i] = 1;
      ++r;
      for (s.push(i); !s.empty();) {
        unsigned j = s.top();
        s.pop();
        for (auto& e : g.Edges(j)) {
          unsigned c = e->Other(j);
          if (visited[c] == 0) {
            visited[c] = 1;
            s.push(c);
          }
        }
      }
    }
  }
  return r;
}
}  // namespace dynamic
}  // namespace graph
