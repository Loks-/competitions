#pragma once

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular.h"

// Calculate (n!/p^k) mod p, where k is the power of p in n!.
// Based on Wilson theorem.
namespace modular {
namespace mstatic {
template <class TModular>
constexpr TModular FactorialExtendedPrimeCoprimeOnly(
    uint64_t n, Factorial<TModular, true>& f, bool inverted = false) {
  constexpr uint64_t p = TModular::GetMod();
  const TModular r =
      inverted ? f.GetI(unsigned(n % p)) : f.Get(unsigned(n % p));
  return ((n / p) & 1) ? -r : r;
}

template <class TModular>
constexpr TModular FactorialExtendedPrime(uint64_t n,
                                          Factorial<TModular, true>& f,
                                          bool inverted = false) {
  if (n == 0) return 1;
  constexpr uint64_t p = TModular::GetMod();
  return FactorialExtendedPrimeCoprimeOnly(n, f, inverted) *
         FactorialExtendedPrime(n / p, f, inverted);
}

template <class TModular>
inline TModular FactorialExtendedPrime(uint64_t n, bool inverted = false) {
  thread_local Factorial<TModular, true> f;
  return FactorialExtendedPrime(n, f, inverted);
}

template <class TModular>
inline TModular BinomialCoefficientExtendedPrime(uint64_t n, uint64_t k,
                                                 bool inverted = false) {
  return FactorialExtendedPrime<TModular>(n, inverted) *
         FactorialExtendedPrime<TModular>(k, !inverted) *
         FactorialExtendedPrime<TModular>(n - k, !inverted);
}
}  // namespace mstatic
}  // namespace modular
