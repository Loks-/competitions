#pragma once

#include "common/base.h"
#include "common/numeric/utils/factorial_prime_power.h"

inline uint64_t BinomialCoefficientPrimePower(uint64_t n, uint64_t k,
                                              uint64_t p) {
  return FactorialPrimePower(n, p) - FactorialPrimePower(k, p) -
         FactorialPrimePower(n - k, p);
}
