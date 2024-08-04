#pragma once

#include "common/modular.h"
#include "common/polynomial/modular/multiplication.h"
#include "common/polynomial/polynomial.h"

namespace polynomial {
template <uint64_t prime, unsigned maxn = (1u << 16)>
inline Polynomial<TModular_P32<prime>> PowU(
    const Polynomial<TModular_P32<prime>>& x, uint64_t pow) {
  Polynomial<TModular_P32<prime>> ans(1u), xx(x);
  for (; pow; pow >>= 1) {
    if (pow & 1) ans = Mult<prime, maxn>(ans, xx);
    xx = Sqr<prime, maxn>(xx);
  }
  return ans;
}
}  // namespace polynomial
