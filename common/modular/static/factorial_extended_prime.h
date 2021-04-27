#pragma once

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular.h"

// Calculate (n!/p^k) mod p, where k is the power of p in n!.
// Based on Wilson theorem.
namespace modular {
namespace mstatic {
template <class TModular>
inline TModular FactorialExtendedPrimeCoprimeOnly(uint64_t n,
                                                  Factorial<TModular, true>& f,
                                                  bool inverted = false) {
  const uint64_t p = TModular::GetMod();
  TModular r = inverted ? f.GetI(unsigned(n % p)) : f.Get(unsigned(n % p));
  return ((n / p) & 1) ? -r : r;
}

template <class TModular>
inline TModular FactorialExtendedPrime(uint64_t n, Factorial<TModular, true>& f,
                                       bool inverted = false) {
  if (n == 0) return 1;
  const uint64_t p = TModular::GetMod();
  return FactorialExtendedPrimeCoprimeOnly(n, f, inverted) *
         FactorialExtendedPrime(n / p, f, inverted);
}

template <class TModular>
inline TModular FactorialExtendedPrime(uint64_t n, bool inverted = false) {
  thread_local Factorial<TModular, true> f;
  return FactorialExtendedPrime(n, f, inverted);
}
}  // namespace mstatic
}  // namespace modular
