#pragma once

#include "common/base.h"
#include "common/graph/graph.h"
#include <stack>
#include <vector>

inline std::vector<uint64_t> ConnectedComponentsMask(
    const UndirectedGraph& graph, uint64_t mask = -1ull) {
  std::vector<uint64_t> components;
  unsigned n = graph.Size();
  assert(n <= 64);
  uint64_t visited = 0;
  std::stack<unsigned> s;
  for (unsigned i = 0; i < n; ++i) {
    uint64_t ibit = (1ull << i);
    if (mask & ibit & ~visited) {
      visited |= ibit;
      uint64_t c = 0;
      for (s.push(i); !s.empty();) {
        unsigned j = s.top();
        s.pop();
        c |= (1ull << j);
        for (unsigned k : graph.Edges(j)) {
          uint64_t kbit = (1ull << k);
          if (mask & kbit & ~visited) {
            visited |= kbit;
            s.push(k);
          }
        }
      }
      components.push_back(c);
    }
  }
  return components;
}
