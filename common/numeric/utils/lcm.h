#pragma once

#include "common/numeric/utils/gcd.h"

template <class T>
constexpr T LCM(const T& x, const T& y) {
  const T d = GCD(x, y);
  return (x / d) * y;
}
