#pragma once

#include "common/base.h"
#include "common/modular/proxy/factorial.h"
#include "common/modular_proxy.h"

// Based on Lucas's theorem.
namespace modular {
namespace proxy {
constexpr uint64_t BinomialCoefficientPrime(uint64_t n, uint64_t k,
                                            Factorial<ModularProxyPrime32>& f) {
  if (k > n) return 0;
  const ModularProxyPrime32& proxy = f.GetProxy();
  const uint64_t p = proxy.GetMod();
  uint64_t r = 1;
  for (; n; n /= p, k /= p) {
    uint64_t np = n % p, kp = k % p;
    if (kp > np) return 0;
    r = proxy.Mult(r, f.BinomialCoefficient(unsigned(np), unsigned(kp)));
  }
  return r;
}

constexpr uint64_t BinomialCoefficientPrime(uint64_t n, uint64_t k,
                                            unsigned prime) {
  Factorial<ModularProxyPrime32> f(prime);
  return BinomialCoefficientPrime(n, k, f);
}
}  // namespace proxy
}  // namespace modular
