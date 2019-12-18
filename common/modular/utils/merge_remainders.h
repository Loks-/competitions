#pragma once

#include "common/modular/arithmetic.h"

// From chinese remainder theorem
template <class TModularAQ = modular::TArithmetic_C32U>
inline uint64_t MergeRemainders(uint64_t p, uint64_t pr, uint64_t q,
                                uint64_t qr) {
  return TModularAQ::Div(TModularAQ::SubSafe(qr, pr, q), p % q, q) * p + pr;
}
