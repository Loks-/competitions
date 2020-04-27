#pragma once

#include "common/base.h"
#include "common/factorization/table/mobius.h"
#include "common/numeric/utils/sign.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/numeric/utils/usqrt.h"

#include <algorithm>
#include <vector>

// Memory: O(U)
// Time Build: O(U * log(log(U)))
// Time Get: O(X^(2/3) * log(log(X)) + U)
namespace factorization {
class MertensCompact {
 protected:
  uint64_t u;
  table::Mobius mobius;

  uint64_t b, e;
  int64_t last_mertens;
  std::vector<int64_t> bmobius;
  std::vector<int64_t> bmertens;

  uint64_t rx, ru;

  int64_t S(uint64_t m) {
    uint64_t y = rx / m, v = std::min(USqrt(y), rx / ru - 1), k = y / (v + 1);
    int64_t s = 0;
    uint64_t ne1 = (b ? std::min(k, y / b) : k) + 1;
    for (uint64_t n = std::max(ru / m, y / e) + 1; n < ne1; ++n)
      s -= bmertens[y / n - b];
    uint64_t ne2 = std::min(v + 1, e);
    for (uint64_t n = std::max<uint64_t>(b, 1); n < ne2; ++n)
      s -= (int64_t(y / n) - int64_t(y / (n + 1))) * bmertens[n - b];
    return s;
  }

  void FirstBlock() {
    b = 0;
    e = u + 1;
    last_mertens = 0;
    bmobius.resize(e);
    bmertens.resize(e);
    for (uint64_t i = 0; i < e; ++i) {
      last_mertens += mobius(i);
      bmertens[i] = last_mertens;
    }
  }

  void NextBlock() {
    b = e;
    e = b + u + 1;
    std::fill(bmobius.begin(), bmobius.end(), 1);
    for (uint64_t p : mobius.GetPrimes()) {
      uint64_t p2 = p * p;
      if (p2 >= e) break;
      for (uint64_t i = p2 * ((b - 1) / p2 + 1); i < e; i += p2)
        bmobius[i - b] = 0;
      for (uint64_t i = p * ((b - 1) / p + 1); i < e; i += p)
        bmobius[i - b] *= -int64_t(p);
    }
    for (uint64_t i = b; i < e; ++i) {
      int64_t m = bmobius[i - b];
      if (m == int64_t(i))
        ++last_mertens;
      else if (m == -int64_t(i))
        --last_mertens;
      else
        last_mertens -= Sign(m);
      bmertens[i - b] = last_mertens;
    }
  }

 public:
  explicit MertensCompact(uint64_t _u) : u(_u), mobius(u) {}

  // x <= U^3
  int64_t GetMertens(uint64_t x) {
    if (x <= u) {
      int64_t s = 0;
      for (uint64_t i = 1; i <= x; ++i) s += mobius(i);
      return s;
    } else {
      rx = x;
      ru = std::min(UCbrt(x), u);
      FirstBlock();
      int64_t s = bmertens[ru];
      for (;; NextBlock()) {
        for (uint64_t n = 1; n <= ru; ++n) {
          int m = mobius(n);
          if (m) s += m * S(n);
        }
        if (e > x / ru) break;
      }
      return s;
    }
  }

  int64_t operator()(uint64_t x) { return GetMertens(x); }
};
}  // namespace factorization
