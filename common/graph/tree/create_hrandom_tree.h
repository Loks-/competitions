#pragma once

#include "common/graph/tree.h"
#include "common/hash/combine.h"

inline TreeGraph CreateHRandomTree(unsigned size, size_t seed = 0) {
  size_t h = seed;
  TreeGraph t(size);
  for (unsigned i = 1; i < size; ++i) {
    nhash::DCombineH(h, i);
    t.AddEdge(i, h % i);
  }
  nhash::DCombineH(h, size);
  t.SetRoot(h % size);
  return t;
}
