#pragma once

#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/unsigned/multiplication.h"

namespace numeric {
namespace nlong {
inline Unsigned PowU(const Unsigned& x, uint64_t pow) {
  Unsigned ans(1u), xx(x);
  for (; pow; pow >>= 1) {
    if (pow & 1) ans = Mult(ans, xx);
    xx = Sqr(xx);
  }
  return ans;
}
}  // namespace nlong
}  // namespace numeric
