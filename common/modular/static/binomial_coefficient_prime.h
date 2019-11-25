#pragma once

#include "common/base.h"
#include "common/modular/static/factorial.h"

// Based on Lucas's theorem.

template <class TModular>
inline TModular ModularPrimeBinomialCoefficient(uint64_t n, uint64_t k,
                                                modular::mstatic::Factorial<TModular>& f) {
  if (k > n) return 0;
  uint64_t p = TModular::GetMod();
  TModular r = 1;
  for (; n; n /= p, k /= p) {
    uint64_t np = n % p, kp = k % p;
    if (kp > np) return 0;
    r *= f.BinomialCoefficient(unsigned(np), unsigned(kp));
  }
  return r;
}

template <class TModular>
inline TModular ModularPrimeBinomialCoefficient(uint64_t n, uint64_t k) {
  thread_local modular::mstatic::Factorial<TModular> f;
  return ModularPrimeBinomialCoefficient(n, k, f);
}
