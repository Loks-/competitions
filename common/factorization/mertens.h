#pragma once

#include "common/base.h"
#include "common/factorization/table/mertens.h"
#include "common/numeric/utils/usqrt.h"

// Memory: O(U)
// Time Build: O(U * log(log(U)))
// Time Get: O(X / (U^1/2))
// Optimal U ~ X^(2/3)
namespace factorization {
class Mertens {
 protected:
  uint64_t u;
  table::Mertens mertens;

 protected:
  // y <= U^2
  // Time: O(sqrt(y))
  constexpr int64_t S(uint64_t y) const {
    const uint64_t v = USqrt(y), k = y / (v + 1);
    int64_t r = 1 + int64_t(k) * mertens(v);
    for (uint64_t n = y / u + 1; n <= k; ++n) r -= mertens(y / n);
    for (uint64_t n = 1; n <= v; ++n)
      r -= int64_t(y / n) * mertens.GetMobius(n);
    return r;
  }

 public:
  constexpr explicit Mertens(uint64_t _u) : u(_u), mertens(u) {}

  // x <= U
  constexpr int GetMobius(uint64_t x) const { return mertens.GetMobius(x); }

  // x <= U^2
  constexpr int64_t GetMertens(uint64_t x) const {
    if (x <= u) return mertens(x);
    int64_t r = 0;
    for (uint64_t n = 1; n <= x / u; ++n) {
      const int m = GetMobius(n);
      if (m) r += m * S(x / n);
    }
    return r;
  }

  constexpr int64_t operator()(uint64_t x) const { return GetMertens(x); }
};
}  // namespace factorization
