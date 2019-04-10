#pragma once

#include "common/base.h"

inline uint64_t DigitsSum(uint64_t x, uint64_t base = 10) {
  uint64_t s = 0;
  for (; x; x /= base) s += x % base;
  return s;
}
