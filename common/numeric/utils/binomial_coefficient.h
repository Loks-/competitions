#pragma once

#include "../modular_arithmetic_proxy.h"
#include "../factorial_modular_proxy.h"

// Based on Lucas's theorem.

inline uint64_t BinomialCoefficient(uint64_t n, uint64_t k, FactorialModularProxy<TModularArithmeticProxy_P32U>& f)
{
    if (k > n) return 0;
    const TModularArithmeticProxy_P32U& proxy = f.GetProxy();
    uint64_t p = proxy.GetMod();
    uint64_t r = 1;
    for (; n; n /= p, k /= p)
    {
        uint64_t np = n % p, kp = k % p;
        if (kp > np) return 0;
        r = proxy.Mult(r, f.BinomialCoefficient(np, kp));
    }
    return r;
}

inline uint64_t BinomialCoefficient(uint64_t n, uint64_t k, unsigned p)
{
    FactorialModularProxy<TModularArithmeticProxy_P32U> f(p);
    return BinomialCoefficient(n, k, f);
}
