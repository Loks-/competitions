#pragma once

#include "common/numeric/utils/abs.h"

template <class T>
constexpr T GCD(const T& x, const T& y) {
  auto xx(x), yy(y);
  for (; yy;) {
    T z = xx % yy;
    xx = yy;
    yy = z;
  }
  return xx;
}

template <class T>
constexpr T GCDSafe(const T& x, const T& y) {
  return GCD(Abs(x), Abs(y));
}
