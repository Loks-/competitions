#pragma once

#include "common/graph/tree_ei.h"
#include "common/hash/combine.h"

template <class TEdgeInfo>
inline graph::TreeEI<TEdgeInfo> CreateHRandomTree(unsigned size,
                                                  size_t max_edge_cost,
                                                  size_t seed = 0) {
  size_t h = seed;
  graph::TreeEI<TEdgeInfo> t(size);
  for (unsigned i = 1; i < size; ++i) {
    nhash::DCombineH(h, i);
    t.AddEdge(i, h % i, h % max_edge_cost);
  }
  nhash::DCombineH(h, size);
  t.SetRoot(h % size);
  return t;
}
