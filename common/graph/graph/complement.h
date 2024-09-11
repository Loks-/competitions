#pragma once

#include "common/graph/graph.h"
#include "common/graph/graph/adjacency_matrix.h"

// Time: O(V^2)
constexpr UndirectedGraph Complement(const UndirectedGraph& g) {
  const unsigned n = g.Size();
  UndirectedGraph gnew(n);
  const auto gam = AdjacencyMatrix(g);
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = i + 1; j < n; ++j) {
      if (!gam[i][j]) gnew.AddEdge(i, j);
    }
  }
  return gnew;
}
