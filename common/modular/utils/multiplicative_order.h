#pragma once

#include "../modular_arithmetic.h"
#include "../../base.h"
#include "../../factorization/base.h"
#include "../../factorization/utils/eulers_totient.h"
#include "../../factorization/utils/factorize.h"

template <class TModular = TModularArithmetic_C32U>
inline uint64_t GetMultiplicativeOrder(uint64_t n, uint64_t u)
{
    u %= n;
    if ((u == 0) || (u == 1))
        return 1;
    uint64_t nt = EulersTotient(n, Factorize(n));
    TFactorization ntf = Factorize(nt);
    uint64_t np = nt;
    for (auto p : ntf)
    {
        for (uint64_t pp = p.first; ((np % pp) == 0) && (TModular::PowU(u, np / pp, n) == 1);) np /= pp;
    }
    return np;
}