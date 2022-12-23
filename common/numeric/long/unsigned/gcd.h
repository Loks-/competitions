#pragma once

#include "common/numeric/long/unsigned.h"

#include <algorithm>

namespace numeric {
namespace nlong {
inline Unsigned GCD(const Unsigned& x, const Unsigned& y) {
  if (x.Empty()) return y;
  if (y.Empty()) return x;
  Unsigned a(x), b(y);
  unsigned p2a = 0, p2b = 0;
  for (; (a % 2u) == 0; ++p2a) a >>= 1;
  for (; (b % 2u) == 0; ++p2b) b >>= 1;
  for (; !a.Empty(); a -= b) {
    for (; (a % 2u) == 0;) a >>= 1;
    if (a < b) a.swap(b);
  }
  return b << std::min(p2a, p2b);
}

inline Unsigned GCDSafe(const Unsigned& x, const Unsigned& y) {
  return GCD(x, y);
}
}  // namespace nlong
}  // namespace numeric
