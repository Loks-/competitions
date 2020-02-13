#pragma once

#include "common/base.h"
#include "common/numeric/long/unsigned.h"

inline uint64_t DigitsSum(LongUnsigned x, unsigned base = 10) {
  uint64_t s = 0;
  for (; !x.Empty(); x /= base) s += x % base;
  return s;
}
