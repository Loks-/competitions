#pragma once

#include "common/numeric/long/unsigned.h"
#include "common/numeric/utils/gcd.h"

#include <algorithm>

inline LongUnsigned GCD(const LongUnsigned& x, const LongUnsigned& y) {
  if (x.Empty()) return y;
  if (y.Empty()) return x;
  LongUnsigned a(x), b(y);
  unsigned p2a = 0, p2b = 0;
  for (; (a % 2u) == 0; ++p2a) a >>= 1;
  for (; (b % 2u) == 0; ++p2b) b >>= 1;
  for (; !a.Empty(); a -= b) {
    for (; (a % 2u) == 0;) a >>= 1;
    if (a < b) a.swap(b);
  }
  return b << std::min(p2a, p2b);
}

inline LongUnsigned GCDSafe(const LongUnsigned& x, const LongUnsigned& y) {
  return GCD(x, y);
}
