#pragma once

#include "common/graph/graph.h"
#include "common/heap/bucket_ukey_map.h"
#include <vector>

inline std::vector<unsigned> DegeneracyOrder(const UndirectedGraph& g) {
  unsigned n = g.Size();
  heap::BucketUKeyMap queue(n);
  for (unsigned i = 0; i < n; ++i) queue.AddNewKey(i, g.Edges(i).size());
  std::vector<unsigned> visited(n, 0), order;
  order.reserve(n);
  for (; !queue.Empty();) {
    auto x = queue.Extract();
    order.push_back(x.key);
    visited[x.key] = 1;
    for (unsigned v : g.Edges(x.key)) {
      if (!visited[v]) queue.SetPriority(v, queue.GetPriority(v) - 1);
    }
  }
  return order;
}
