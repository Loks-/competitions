#pragma once

#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/ulog.h"

template <class T>
inline T ConcatNumbers(const T& l, const T& r, const T& base = T(10u)) {
  return l * PowU(base, ULogK(r, base) + 1) + r;
}
