#pragma once

#include "common/graph/graph_ei.h"
#include "common/hash.h"

template <class TEdgeInfo, bool directed_edges>
inline graph::GraphEI<TEdgeInfo, directed_edges> CreateHRandomGraph(
    unsigned size, unsigned edges_per_node, size_t max_edge_cost,
    size_t seed = 0) {
  size_t h = seed;
  graph::GraphEI<TEdgeInfo, directed_edges> g(size);
  for (unsigned i = 0; i < size; ++i) {
    for (unsigned j = 0;
         j < (directed_edges ? edges_per_node : (edges_per_node + (i % 2)) / 2);
         ++j) {
      h = HashCombine(h, i + j);
      g.AddEdge(i, h % size, h % max_edge_cost);
    }
  }
  return g;
}
