#pragma once

#include "../factorial_modular_proxy.h"
#include "../modular_arithmetic_proxy.h"

// Calculate (n!/p^k) mod p, where k is the power of p in n!.
// Based on Wilson theorem.

inline uint64_t FactorialModularPrimeCoprimeOnly(uint64_t n, FactorialModularProxy<TModularArithmeticProxy_P32U, false>& f)
{
    const TModularArithmeticProxy_P32U& mp = f.GetProxy();
    uint64_t p = mp.GetMod();
    uint64_t r = f.Get(n % p);
    if ((n / p) & 1) r = mp.Minus(r);
    return r;
}

inline uint64_t FactorialModularPrime(uint64_t n, FactorialModularProxy<TModularArithmeticProxy_P32U, false>& f)
{
    if (n == 0) return 1;
    const TModularArithmeticProxy_P32U& mp = f.GetProxy();
    return mp.Mult(FactorialModularPrimeCoprimeOnly(n, f), FactorialModularPrime(n / p, f));
}

inline uint64_t FactorialModularPrime(uint64_t n, unsigned prime)
{
    FactorialModularProxy<TModularArithmeticProxy_P32U> f(prime);
    return FactorialModularPrime(n, f);
}
