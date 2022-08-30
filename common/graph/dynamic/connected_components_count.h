#pragma once

#include "common/graph/dynamic/graph.h"

#include <stack>
#include <vector>

namespace graph {
namespace dynamic {
// Time: O(V + E)
template <class TData>
inline unsigned ConnectedComponentsCount(const Graph<TData>& graph) {
  unsigned n = graph.Size(), r = 0;
  std::vector<unsigned> visited(n, 0);
  std::stack<unsigned> s;
  for (unsigned i = 0; i < n; ++i) {
    if (visited[i] == 0) {
      visited[i] = 1;
      ++r;
      for (s.push(i); !s.empty();) {
        unsigned j = s.top();
        s.pop();
        for (auto& e : graph.Edges(j)) {
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
