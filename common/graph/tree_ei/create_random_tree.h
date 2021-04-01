#pragma once

#include "common/graph/tree_ei.h"
#include "common/hash.h"

template <class TEdgeInfo>
inline graph::TreeEI<TEdgeInfo> CreateRandomTree(
    unsigned size, size_t max_edge_cost, size_t seed = 0) {
  size_t h = seed;
  graph::TreeEI<TEdgeInfo> t(size);
  for (unsigned i = 1; i < size; ++i) {
    h = HashCombine(h, i);
    t.AddEdge(i, h % i, h % max_edge_cost);
  }
  h = HashCombine(h, size);
  t.SetRoot(h % size);
  return t;
}
