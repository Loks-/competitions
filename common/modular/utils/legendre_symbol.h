#pragma once

#include "common/modular/arithmetic.h"

template <class TModularA = modular::TArithmetic_P32U>
constexpr int LegendreSymbol(uint64_t a, uint64_t p) {
  a %= p;
  if (p <= 2) return int(a);
  const uint64_t t = TModularA::PowU(a, (p - 1) / 2, p);
  return (t <= 1) ? int(t) : -1;
}
