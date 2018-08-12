#pragma once

#include "modular_arithmetic.h"
#include "primes_list.h"

namespace {

template <class TModular = TModularArithmetic_P32U>
uint64_t FindSmallestPrimitive(uint64_t p, const PrimesList& pl)
{
    if (p <= 3)
        return p - 1;
    vector<pair<uint64_t, unsigned>> vp = pl.Factorize(p - 1);
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

} // namespace
