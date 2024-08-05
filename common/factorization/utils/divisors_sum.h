#pragma once

#include "common/factorization/base.h"
#include "common/numeric/utils/pow.h"

constexpr uint64_t DivisorsSum(const TFactorization& factorization) {
  uint64_t s = 1;
  for (auto p : factorization) {
    s *= ((p.power == 1) ? p.prime + 1
                         : (PowU(p.prime, p.power + 1) - 1) / (p.prime - 1));
  }
  return s;
}
