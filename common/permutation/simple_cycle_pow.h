#pragma once

#include "permutation.h"
#include "../base.h"
#include "../numeric/utils/gcd.h"

void PermutationSimpleCyclePow(Permutation::TCycle& cycle, unsigned pow) {
  unsigned size = cycle.size();
  if ((size == 1) || (pow % size == 1)) return;
  unsigned k = GCD(size, pow);
  assert(k == 1);
  thread_local Permutation::TCycle t;
  t.resize(size);
  for (unsigned i = 0; i < size; ++i) t[i] = cycle[(i * pow) % size];
  cycle = t;
}