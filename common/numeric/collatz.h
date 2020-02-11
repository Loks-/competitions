#pragma once

#include "common/base.h"
#include <vector>

// Time to reach 1 in Collatz sequence
namespace numeric {
class Collatz {
 protected:
  std::vector<unsigned> vt;

 public:
  Collatz(unsigned cache_size) : vt(cache_size, 0) { vt[0] = vt[1] = 1; }

  static uint64_t Next(uint64_t n) { return (n & 1) ? (3 * n + 1) : n / 2; }

 protected:
  unsigned GetIS(uint64_t n) {
    if (vt[n]) return vt[n];
    return vt[n] = GetI(Next(n)) + 1;
  }

  unsigned GetI(uint64_t n) {
    unsigned s = 0;
    for (; n >= vt.size(); ++s) n = Next(n);
    return GetIS(n) + s;
  }

 public:
  unsigned Get(uint64_t n) { return GetI(n) - 1; }

  uint64_t operator()(uint64_t n) { return Get(n); }
};
}  // namespace numeric
