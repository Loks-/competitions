#pragma once

#include "common/graph/graph_ei.h"
#include "common/vector/enumerate.h"
#include "common/vector/shuffle.h"

template <class TEdgeInfo, bool directed_edges>
inline graph::GraphEI<TEdgeInfo, directed_edges> CreateLongPathGraph(
    unsigned size, size_t max_edge_cost) {
  auto v = nvector::Enumerate(0u, size);
  nvector::Shuffle(v);
  graph::GraphEI<TEdgeInfo, directed_edges> g(size);
  unsigned block_size = std::min<unsigned>(max_edge_cost / 2 + 1, size),
           nblocks = (size - 1) / block_size + 1;
  for (unsigned bs = 0, b = 0; b < nblocks; ++b) {
    unsigned cbsize = (b + 1 < nblocks) ? block_size : size - bs;
    for (unsigned i = 0; i < cbsize; ++i) {
      for (unsigned j = cbsize - 1; j > i; --j)
        g.AddEdge(v[bs + i], v[bs + j], 2 * (j - i) - 1);
    }
    bs += cbsize;
  }
  for (unsigned b = 1; b < nblocks; ++b)
    g.AddEdge(v[b * block_size - 1], v[b * block_size], 1u);
  g.AddEdge(v.back(), v[0], 1);
  return g;
}
