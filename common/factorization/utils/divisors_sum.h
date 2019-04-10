#pragma once

#include "common/factorization/base.h"
#include "common/numeric/utils/pow.h"

inline uint64_t GetDivisorsSum(const TFactorization& factorization) {
  uint64_t s = 1;
  for (auto p : factorization) {
    s *= ((p.second == 1) ? p.first + 1
                          : (PowU(p.first, p.second + 1) - 1) / (p.first - 1));
  }
  return s;
}
