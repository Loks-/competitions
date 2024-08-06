#pragma once

#include "common/base.h"
#include "common/numeric/long/unsigned.h"

constexpr uint64_t DigitsSum(const LongUnsigned& x, unsigned base = 10) {
  uint64_t s = 0;
  LongUnsigned xx(x);
  for (; !xx.Empty(); xx /= base) s += xx % base;
  return s;
}
