#pragma once

#include "factorial.h"
#include "modular.h"

// Calculate (n!/p^k) mod p, where k is the power of p in n!.
// Based on Wilson theorem.

template<class TModular>
inline TModular ModularFactorialExtendedPrimeCoprimeOnly(uint64_t n, ModularFactorial<TModular, true>& f, bool inverted = false)
{
    const uint64_t p = TModular::GetMod();
    TModular r = inverted ? f.GetI(unsigned(n % p)) : f.Get(unsigned(n % p));
    return ((n / p) & 1) ? -r : r;
}

template<class TModular>
inline TModular ModularFactorialExtendedPrime(uint64_t n, ModularFactorial<TModular, true>& f, bool inverted = false)
{
    if (n == 0) return 1;
    const uint64_t p = TModular::GetMod();
    return ModularFactorialExtendedPrimeCoprimeOnly(n, f, inverted) * ModularFactorialExtendedPrime(n / p, f, inverted);
}

template<class TModular>
inline TModular ModularFactorialExtendedPrime(uint64_t n)
{
    static ModularFactorial<TModular, true> f;
    return ModularFactorialExtendedPrime(n, f);
}
