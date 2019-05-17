#pragma once

#include "common/factorization/base.h"

inline TFactorization FactorizeBase(uint64_t n) {
  TFactorization output;
  unsigned p2 = 0;
  for (; (n % 2) == 0; n /= 2) ++p2;
  if (p2) output.push_back({2ull, p2});
  for (uint64_t p = 3; p * p <= n; p += 2) {
    if ((n % p) == 0) {
      n /= p;
      unsigned cnt = 1;
      for (; (n % p) == 0; ++cnt) n /= p;
      output.push_back({p, cnt});
    }
  }
  if (n != 1) output.push_back({n, 1});
  return output;
}
