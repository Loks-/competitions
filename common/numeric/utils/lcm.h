#pragma once

#include "common/numeric/utils/gcd.h"

template <class T>
inline T LCM(T x, T y) {
  T d = GCD(x, y);
  return (x / d) * y;
}
