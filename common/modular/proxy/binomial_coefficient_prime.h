#pragma once

#include "common/base.h"
#include "common/modular/proxy/factorial.h"
#include "common/modular/proxy/modular_proxy.h"

// Based on Lucas's theorem.

inline uint64_t PModularPrimeBinomialCoefficient(
    uint64_t n, uint64_t k, PModularFactorial<TModularProxy_P32U>& f) {
  if (k > n) return 0;
  const TModularProxy_P32U& proxy = f.GetProxy();
  uint64_t p = proxy.GetMod();
  uint64_t r = 1;
  for (; n; n /= p, k /= p) {
    uint64_t np = n % p, kp = k % p;
    if (kp > np) return 0;
    r = proxy.Mult(r, f.BinomialCoefficient(unsigned(np), unsigned(kp)));
  }
  return r;
}

inline uint64_t PModularPrimeBinomialCoefficient(uint64_t n, uint64_t k,
                                                 unsigned prime) {
  PModularFactorial<TModularProxy_P32U> f(prime);
  return PModularPrimeBinomialCoefficient(n, k, f);
}
