#pragma once

#include "common/graph/graph.h"

#include <stack>
#include <vector>

// Time: O(V + E)
inline std::vector<unsigned> ConnectedComponents(const UndirectedGraph& graph) {
  unsigned n = graph.Size(), l = -1;
  std::vector<unsigned> visited(n, 0), components(n);
  std::stack<unsigned> s;
  for (unsigned i = 0; i < n; ++i) {
    if (visited[i] == 0) {
      visited[i] = 1;
      ++l;
      for (s.push(i); !s.empty();) {
        unsigned j = s.top();
        s.pop();
        components[j] = l;
        for (unsigned c : graph.Edges(j)) {
          if (visited[c] == 0) {
            visited[c] = 1;
            s.push(c);
          }
        }
      }
    }
  }
  return components;
}
