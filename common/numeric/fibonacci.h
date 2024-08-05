#pragma once

#include "common/base.h"

#include <vector>

namespace numeric {
class Fibonacci {
 protected:
  std::vector<uint64_t> vf;

 public:
  constexpr Fibonacci() : vf({0, 1}) {}

  constexpr void Adjust(unsigned n) {
    unsigned i = unsigned(vf.size());
    for (; i <= n; ++i) vf.push_back(vf[i - 2] + vf[i - 1]);
  }

  constexpr void AdjustMaxValue(uint64_t n) {
    unsigned i = unsigned(vf.size() - 1);
    for (; vf[i] < n; ++i) vf.push_back(vf[i - 1] + vf[i]);
  }

  constexpr size_t Size() const { return vf.size(); }

  constexpr uint64_t Get(unsigned n) {
    Adjust(n);
    return vf[n];
  }

  constexpr uint64_t operator()(unsigned n) { return Get(n); }
};
}  // namespace numeric

inline uint64_t GetFibonacci(unsigned n) {
  thread_local numeric::Fibonacci f;
  return f(n);
}
