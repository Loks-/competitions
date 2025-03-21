#pragma once

#include "common/modular.h"
#include "common/polynomial/modular/multiplication.h"
#include "common/polynomial/polynomial.h"

namespace polynomial {
template <uint64_t prime>
inline Polynomial<ModularPrime32<prime>> PowU(
    const Polynomial<ModularPrime32<prime>>& x, uint64_t pow) {
  Polynomial<ModularPrime32<prime>> ans(1u), xx(x);
  for (; pow; pow >>= 1) {
    if (pow & 1) ans = Mult<prime>(ans, xx);
    xx = Sqr<prime>(xx);
  }
  return ans;
}
}  // namespace polynomial
