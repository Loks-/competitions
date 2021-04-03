#pragma once

#include "common/graph/tree.h"
#include "common/hash.h"

inline TreeGraph CreateRandomTree(unsigned size, size_t seed = 0) {
  size_t h = seed;
  TreeGraph t(size);
  for (unsigned i = 1; i < size; ++i) {
    h = HashCombine(h, i);
    t.AddEdge(i, h % i);
  }
  h = HashCombine(h, size);
  t.SetRoot(h % size);
  return t;
}
