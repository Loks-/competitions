#pragma once

#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/utils/factorial.h"

inline LongUnsigned GetBinomialCoefficientL(unsigned n, unsigned k) {
  if (k > n) return LongUnsigned(0);
  return GetFactorialL(n) / GetFactorialL(k) / GetFactorialL(n - k);
}
