#pragma once

#include "common/graph/graph.h"
#include <stack>
#include <vector>

namespace graph {
using TConnectedComponents = std::vector<std::vector<unsigned>>;

inline TConnectedComponents ConnectedComponents(const UndirectedGraph& graph) {
  TConnectedComponents components;
  unsigned n = graph.Size();
  std::vector<unsigned> visited(n, 0);
  std::stack<unsigned> s;
  for (unsigned i = 0; i < n; ++i) {
    if (visited[i] == 0) {
      visited[i] = 1;
      components.push_back({});
      std::vector<unsigned>& component = components.back();
      for (s.push(i); !s.empty();) {
        unsigned j = s.top();
        s.pop();
        component.push_back(j);
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
}  // namespace graph
