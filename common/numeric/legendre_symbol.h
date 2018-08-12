#pragma once

#include "modular_arithmetic.h"

namespace {

template <class TModular = TModularArithmetic_P32U>
int LegendreSymbol(uint64_t a, uint64_t p)
{
    a %= p;
    if (p <= 2)
        return int(a);
    uint64_t t = TModular::PowU(a, (p - 1) / 2, p);
    return (t <= 1) ? int(t) : -1;
}

} // namespace
