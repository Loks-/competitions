#pragma once

#include "../arithmetic.h"

template <class TModularA = TModularArithmetic_P32U>
inline int LegendreSymbol(uint64_t a, uint64_t p)
{
    a %= p;
    if (p <= 2)
        return int(a);
    uint64_t t = TModularA::PowU(a, (p - 1) / 2, p);
    return (t <= 1) ? int(t) : -1;
}
