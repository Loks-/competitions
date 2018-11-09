#pragma once

#include "factorial.h"
#include "modular_proxy.h"

// Calculate (n!/p^k) mod p, where k is the power of p in n!.
// Based on Wilson theorem.

inline uint64_t PModularFactorialExtendedPrimeCoprimeOnly(uint64_t n, PModularFactorial<TModularProxy_P32U, false>& f)
{
    const TModularProxy_P32U& mp = f.GetProxy();
    uint64_t p = mp.GetMod();
    uint64_t r = f.Get(n % p);
    if ((n / p) & 1) r = mp.Minus(r);
    return r;
}

inline uint64_t PModularFactorialExtendedPrime(uint64_t n, PModularFactorial<TModularProxy_P32U, false>& f)
{
    if (n == 0) return 1;
    const TModularProxy_P32U& mp = f.GetProxy();
    uint64_t p = mp.GetMod();
    return mp.Mult(PModularFactorialExtendedPrimeCoprimeOnly(n, f), PModularFactorialExtendedPrime(n / p, f));
}

inline uint64_t PModularFactorialExtendedPrime(uint64_t n, unsigned prime)
{
    PModularFactorial<TModularProxy_P32U, false> f(prime);
    return PModularFactorialExtendedPrime(n, f);
}
