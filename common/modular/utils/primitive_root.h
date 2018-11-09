#pragma once

#include "../arithmetic.h"
#include "../../base.h"
#include "../../factorization/base.h"

template <class TModularA = TModularArithmetic_P32U>
inline uint64_t FindSmallestPrimitive(uint64_t p, const TFactorization& p1_factorization)
{
    if (p <= 3)
        return p - 1;
    for (uint64_t u = 2; u < p; ++u)
    {
        bool good = true;
        for (unsigned i = 0; i < p1_factorization.size(); ++i)
        {
            uint64_t t = TModularA::PowU(u, (p - 1) / p1_factorization[i].first, p);
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
