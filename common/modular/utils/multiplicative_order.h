#pragma once

#include "common/base.h"
#include "common/factorization/factorization.h"
#include "common/factorization/utils/eulers_totient.h"
#include "common/modular/arithmetic.h"

template <class TModularA = TModularArithmetic_C32U>
inline uint64_t GetMultiplicativeOrder(uint64_t n, uint64_t u) {
  u %= n;
  if ((u == 0) || (u == 1)) return 1;
  uint64_t nt = EulersTotient(n, Factorize(n));
  TFactorization ntf = Factorize(nt);
  uint64_t np = nt;
  for (auto p : ntf) {
    for (uint64_t pp = p.prime;
         ((np % pp) == 0) && (TModularA::PowU(u, np / pp, n) == 1);)
      np /= pp;
  }
  return np;
}
