#pragma once

#include "common/base.h"

inline uint64_t Reverse(uint64_t n, uint64_t base = 10) {
  uint64_t r = 0;
  for (; n; n /= base) r = base * r + (n % base);
  return r;
}
