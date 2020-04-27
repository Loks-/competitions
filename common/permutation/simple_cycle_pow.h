#pragma once

#include "common/base.h"
#include "common/numeric/utils/gcd.h"
#include "common/permutation/base.h"

namespace permutation {
inline void SimpleCyclePow(TCycle& cycle, unsigned pow) {
  unsigned size = cycle.size();
  if ((size == 1) || (pow % size == 1)) return;
  assert(GCD(size, pow) == 1);
  thread_local TCycle t;
  t.resize(size);
  for (unsigned i = 0; i < size; ++i) t[i] = cycle[(i * pow) % size];
  cycle.swap(t);
}
}  // namespace permutation
