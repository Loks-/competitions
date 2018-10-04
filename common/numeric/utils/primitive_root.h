#pragma once

#include "../modular_arithmetic.h"
#include "../primes_list.h"
#include "../../base.h"

template <class TModular = TModularArithmetic_P32U>
inline uint64_t FindSmallestPrimitive(uint64_t p, const PrimesList& primes_list)
{
    if (p <= 3)
        return p - 1;
    PrimesList::TFactorization vp = primes_list.Factorize(p - 1);
    for (uint64_t u = 2; u < p; ++u)
    {
        bool good = true;
        for (unsigned i = 0; i < vp.size(); ++i)
        {
            uint64_t t = TModular::PowU(u, (p - 1) / vp[i].first, p);
            if (t == 1)
            {
                good = false;
                break;
            }
        }
        if (good)
            return u;
    }
    assert(false);
    return 0;
}
