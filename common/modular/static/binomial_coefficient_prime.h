#pragma once

#include "common/base.h"
#include "common/modular/static/factorial.h"

// Based on Lucas's theorem.

namespace modular {
namespace mstatic {
template <class TModular>
inline TModular BinomialCoefficientPrime(uint64_t n, uint64_t k,
                                         Factorial<TModular>& f) {
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
inline TModular BinomialCoefficientPrime(uint64_t n, uint64_t k) {
  thread_local Factorial<TModular> f;
  return BinomialCoefficientPrime(n, k, f);
}
}  // namespace mstatic
}  // namespace modular
