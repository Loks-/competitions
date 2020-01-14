#pragma once

#include "common/base.h"

template <class T>
inline T PowU(T x, uint64_t pow) {
  T ans = T(1);
  for (; pow; pow >>= 1) {
    if (pow & 1) ans *= x;
    x *= x;
  }
  return ans;
}

template <class T>
inline T PowS(T x, int64_t pow) {
  return (pow < 0) ? PowU(T(1) / x, -pow) : PowU(x, pow);
}
