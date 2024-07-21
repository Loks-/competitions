#pragma once

#include "common/graph/graph.h"

#include <stack>
#include <vector>

// Time: O(V + E)
inline unsigned ConnectedComponentsCount(const UndirectedGraph& g) {
  unsigned n = g.Size(), r = 0;
  std::vector<unsigned> visited(n, 0);
  std::stack<unsigned> s;
  for (unsigned i = 0; i < n; ++i) {
    if (visited[i] == 0) {
      visited[i] = 1;
      ++r;
      for (s.push(i); !s.empty();) {
        unsigned j = s.top();
        s.pop();
        for (unsigned c : g.Edges(j)) {
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
