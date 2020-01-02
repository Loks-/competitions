#pragma once

#include "common/factorization/table/mobius.h"
#include "common/numeric/utils/sign.h"
#include "common/numeric/utils/usqrt.h"
#include <algorithm>
#include <vector>

#include <iostream>

// Memory: O(U)
// Time Build: O(U * log(log(U)))
// Time Get: ??? O(X / (U^1/2))
// Optimal U ~ X^(1/3)
namespace factorization {
class MertensCompact {
 protected:
  uint64_t u;
  table::Mobius mobius;

  uint64_t b, e, last_mertens;
  std::vector<int64_t> vmobius;
  std::vector<int> vmertens;

  bool Valid(uint64_t t) const { return (b <= t) && (t < e); }

  int S(uint64_t x, uint64_t m) {
    uint64_t y = x / m, v = std::min(USqrt(y), x / u - 1), k = y / (v + 1);
    int r = Valid(v) ? 1 : 0;
    uint64_t ne1 = (b ? std::min(k, y / b) : k) + 1;
    for (uint64_t n = std::max(u / m, y / e) + 1; n < ne1; ++n)
      r -= vmertens[y / n - b];
    uint64_t ne2 = std::min(v + 1, e);
    for (uint64_t n = std::max(b, uint64_t(1)); n < ne2; ++n)
      r -= (int(y / n) - int(y / (n + 1))) * vmertens[n - b];
    return r;
  }

  void FirstBlock() {
    b = 0;
    e = u + 1;
    last_mertens = 0;
    vmobius.resize(e);
    vmertens.resize(e);
    for (uint64_t i = 0; i < e; ++i) {
      vmobius[i] = mobius(i);
      last_mertens += int(vmobius[i]);
      vmertens[i] = last_mertens;
    }
  }

  void NextBlock() {
    b = e;
    e = b + u + 1;
    std::fill(vmobius.begin(), vmobius.end(), 1);
    for (uint64_t p : mobius.GetPrimes()) {
      uint64_t p2 = p * p;
      if (p2 >= e) break;
      for (uint64_t i = p2 * ((b - 1) / p2 + 1); i < e; i += p2)
        vmobius[i - b] = 0;
      for (uint64_t i = p * ((b - 1) / p + 1); i < e; i += p)
        vmobius[i - b] *= -int64_t(p);
    }
    for (uint64_t i = b; i < e; ++i) {
      int64_t& m = vmobius[i - b];
      if (m == int64_t(i))
        m = 1;
      else if (m == -int64_t(i))
        m = -1;
      else
        m = -Sign(m);
      last_mertens += int(m);
      vmertens[i - b] = last_mertens;
    }
  }

 public:
  MertensCompact(unsigned _u) : u(_u), mobius(u) {}

  // x <= U^3
  int GetMertens(uint64_t x) {
    if (x <= u) {
      int s = 0;
      for (uint64_t i = 1; i <= x; ++i) s += mobius(i);
      return s;
    } else {
      uint64_t xsqrt = USqrt(x);
      int s = 0;
      for (FirstBlock(); (b <= x / u) || (b <= xsqrt); NextBlock()) {
        for (uint64_t n = 1; n <= u; ++n) {
          int m = mobius(n);
          if (m) s += m * S(x, n);
        }
      }
      return s;
    }
  }

  int operator()(uint64_t x) { return GetMertens(x); }
};
}  // namespace factorization
