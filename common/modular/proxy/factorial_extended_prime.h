#pragma once

#include "common/modular/proxy/factorial.h"
#include "common/modular_proxy.h"

// Calculate (n!/p^k) mod p, where k is the power of p in n!.
// Based on Wilson theorem.

namespace modular {
namespace proxy {
constexpr uint64_t FactorialExtendedPrimeCoprimeOnly(
    uint64_t n, Factorial<TModularProxy_P32U, false>& f) {
  const TModularProxy_P32U& mp = f.GetProxy();
  uint64_t p = mp.GetMod();
  uint64_t r = f.Get(n % p);
  if ((n / p) & 1) r = mp.Minus(r);
  return r;
}

constexpr uint64_t FactorialExtendedPrime(
    uint64_t n, Factorial<TModularProxy_P32U, false>& f) {
  if (n == 0) return 1;
  const TModularProxy_P32U& mp = f.GetProxy();
  uint64_t p = mp.GetMod();
  return mp.Mult(FactorialExtendedPrimeCoprimeOnly(n, f),
                 FactorialExtendedPrime(n / p, f));
}

constexpr uint64_t FactorialExtendedPrime(uint64_t n, unsigned prime) {
  Factorial<TModularProxy_P32U, false> f(prime);
  return FactorialExtendedPrime(n, f);
}
}  // namespace proxy
}  // namespace modular
