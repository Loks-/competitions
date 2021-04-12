#pragma once

#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/utils/factorial.h"

inline LongUnsigned GetBinomialCoefficientL(uint32_t n, uint32_t k) {
  if (k > n) return LongUnsigned(0);
  if (k > n - k) k = n - k;
  LongUnsigned l(1u);
  for (uint32_t i = 1; i <= k; ++i) l = (l * (n - k + i)) / i;
  return l;
}
