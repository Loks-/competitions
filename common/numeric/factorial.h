#pragma once

#include "common/base.h"
#include <vector>

class Factorial {
 protected:
  std::vector<uint64_t> vf;

 public:
  Factorial() { vf.push_back(1); }

  void Adjust(unsigned n) {
    for (; vf.size() <= n;) vf.push_back(vf.size() * vf.back());
  }

  uint64_t Get(unsigned n) {
    Adjust(n);
    return vf[n];
  }

  uint64_t operator()(unsigned n) { return Get(n); }

  uint64_t BinomialCoefficient(unsigned n, unsigned k) {
    if (k > n) return 0;
    Adjust(n);
    return vf[n] / (vf[k] * vf[n - k]);
  }
};
