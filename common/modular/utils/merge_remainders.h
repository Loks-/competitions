#pragma once

#include "../modular_arithmetic.h"

// From chinese remainder theorem
template<class TModularQ = TModularArithmetic_C32U>
static uint64_t MergeRemainders(uint64_t p, uint64_t pr, uint64_t q, uint64_t qr)
{
    return TModularQ::Div(TModularQ::SubSafe(qr, pr, q), p % q, q) * p + pr;
}
