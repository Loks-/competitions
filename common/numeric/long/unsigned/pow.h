#pragma once

#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/unsigned/multiplication.h"

namespace numeric {
namespace nlong {
template <unsigned log2_maxn = 16>
inline Unsigned PowU(const Unsigned& x, uint64_t pow) {
  Unsigned ans(1u), xx(x);
  for (; pow; pow >>= 1) {
    if (pow & 1) ans = Mult<log2_maxn>(ans, xx);
    xx = Sqr<log2_maxn>(xx);
  }
  return ans;
}
}  // namespace nlong
}  // namespace numeric
