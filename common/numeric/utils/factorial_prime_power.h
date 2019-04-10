#pragma once

#include "common/base.h"

inline uint64_t FactorialPrimePower(uint64_t n, uint64_t p) {
  uint64_t s = 0;
  for (n /= p; n; n /= p) s += n;
  return s;
}
