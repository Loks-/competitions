#pragma once

#include "common/base.h"

template <class T>
inline T PowU(const T& x, uint64_t pow) {
  T ans = T(1), xx(x);
  for (; pow; pow >>= 1) {
    if (pow & 1) ans *= xx;
    xx *= xx;
  }
  return ans;
}

template <class T>
inline T PowS(const T& x, int64_t pow) {
  return (pow < 0) ? PowU(T(1) / x, -pow) : PowU(x, pow);
}
