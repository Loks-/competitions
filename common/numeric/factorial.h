#pragma once

#include "common/base.h"

#include <vector>

namespace numeric {
class Factorial {
 protected:
  std::vector<uint64_t> vf;

 public:
  constexpr Factorial() { vf.push_back(1); }

  constexpr void Adjust(unsigned n) {
    unsigned k = vf.size();
    if (k > n) return;
    vf.resize(n + 1);
    for (unsigned i = k; i <= n; ++i) vf[i] = vf[i - 1] * i;
  }

  constexpr uint64_t Get(unsigned n) {
    Adjust(n);
    return vf[n];
  }

  constexpr uint64_t operator()(unsigned n) { return Get(n); }

  constexpr uint64_t BinomialCoefficient(unsigned n, unsigned k) {
    if (k > n) return 0;
    Adjust(n);
    return vf[n] / (vf[k] * vf[n - k]);
  }
};
}  // namespace numeric

inline uint64_t GetFactorial(unsigned n) {
  thread_local numeric::Factorial f;
  return f(n);
}
