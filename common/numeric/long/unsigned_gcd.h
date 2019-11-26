#pragma once

#include "common/numeric/long/unsigned.h"
#include <algorithm>

LongUnsigned GCD(LongUnsigned a, LongUnsigned b) {
  if (a.Empty()) return b;
  if (b.Empty()) return a;
  unsigned p2a = 0, p2b = 0;
  for (; (a % 2u) == 0; ++p2a) a >>= 1;
  for (; (b % 2u) == 0; ++p2b) b >>= 1;
  for (; !a.Empty(); a -= b) {
    for (; (a % 2u) == 0;) a >>= 1;
    if (a < b) a.swap(b);
  }
  return b << std::min(p2a, p2b);
}
