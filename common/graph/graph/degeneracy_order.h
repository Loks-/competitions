#pragma once

#include "common/graph/graph.h"
#include "common/heap/ukvm/bucket_queue.h"

#include <vector>

// Time: O(V + E)
inline std::vector<unsigned> DegeneracyOrder(const UndirectedGraph& g) {
  const unsigned n = g.Size();
  heap::ukvm::BucketQueue queue(n);
  for (unsigned i = 0; i < n; ++i) queue.AddNewKey(i, g.Edges(i).size());
  std::vector<unsigned> order;
  order.reserve(n);
  for (; !queue.Empty();) {
    const unsigned u = queue.ExtractKey();
    order.push_back(u);
    for (unsigned v : g.Edges(u)) {
      if (queue.InHeap(v)) queue.DecreaseValue(v, queue.Get(v) - 1);
    }
  }
  return order;
}
