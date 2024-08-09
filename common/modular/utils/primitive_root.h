#pragma once

#include "common/base.h"
#include "common/factorization/base.h"
#include "common/modular/arithmetic.h"

template <class TModularA = modular::TArithmetic_P32U>
constexpr uint64_t FindSmallestPrimitiveRoot(
    uint64_t p, const TFactorization& p1_factorization) {
  if (p <= 3) return p - 1;
  for (uint64_t u = 2; u < p; ++u) {
    bool good = true;
    for (unsigned i = 0; i < p1_factorization.size(); ++i) {
      if (TModularA::PowU(u, (p - 1) / p1_factorization[i].prime, p) == 1) {
        good = false;
        break;
      }
    }
    if (good) return u;
  }
  assert(false);
  return 0;
}
