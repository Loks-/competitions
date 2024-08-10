#pragma once

#include "common/base.h"
#include "common/factorization/base.h"
#include "common/modular/arithmetic.h"

template <class TModularA = modular::TArithmetic_P32U>
constexpr bool IsPrimitiveRoot(uint64_t p,
                               const TFactorization& p1_factorization,
                               unsigned u) {
  for (const auto& fp : p1_factorization) {
    if (TModularA::PowU(u, (p - 1) / fp.prime, p) == 1) return false;
  }
  return true;
}

template <class TModularA = modular::TArithmetic_P32U>
constexpr uint64_t FindSmallestPrimitiveRoot(
    uint64_t p, const TFactorization& p1_factorization) {
  if (p <= 3) return p - 1;
  for (uint64_t u = 2; u < p; ++u) {
    if (IsPrimitiveRoot(p, p1_factorization, u)) return u;
  }
  assert(false);
  return 0;
}
