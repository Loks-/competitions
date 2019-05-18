#pragma once

#include "common/graph/graph.h"
#include <vector>

template <bool directed_edges>
inline std::vector<std::vector<bool>> AdjacencyMatrix(
    const graph::Graph<directed_edges>& g) {
  unsigned n = g.Size();
  std::vector<std::vector<bool>> v(n, std::vector<bool>(n, false));
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j : g.Edges(i)) v[i][j] = true;
  }
  return v;
}
