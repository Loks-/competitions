#pragma once

#include "../arithmetic.h"
#include "../../base.h"
#include "../../factorization/factorization.h"
#include "../../factorization/utils/eulers_totient.h"

template <class TModularA = TModularArithmetic_C32U>
inline uint64_t GetMultiplicativeOrder(uint64_t n, uint64_t u) {
  u %= n;
  if ((u == 0) || (u == 1)) return 1;
  Factorization ff;
  uint64_t nt = EulersTotient(n, ff.Factorize(n));
  TFactorization ntf = ff.Factorize(nt);
  uint64_t np = nt;
  for (auto p : ntf) {
    for (uint64_t pp = p.first;
         ((np % pp) == 0) && (TModularA::PowU(u, np / pp, n) == 1);)
      np /= pp;
  }
  return np;
}
