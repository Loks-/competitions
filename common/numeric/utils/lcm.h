#pragma once

#include "common/numeric/utils/gcd.h"

template <class T>
inline T LCM(const T& x, const T& y) {
  T d = GCD(x, y);
  return (x / d) * y;
}
