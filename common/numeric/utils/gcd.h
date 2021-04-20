#pragma once

#include "common/numeric/utils/abs.h"

template <class T>
inline T GCD(T x, T y) {
  for (; y;) {
    T z = x % y;
    x = y;
    y = z;
  }
  return x;
}

template <class T>
inline T GCDSafe(T x, T y) {
  return GCD<T>(Abs(x), Abs(y));
}
