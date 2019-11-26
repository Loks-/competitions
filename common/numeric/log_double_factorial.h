#pragma once

#include "common/numeric/log_double.h"
#include <vector>

namespace numeric {
class LogDoubleFactorial {
 protected:
  std::vector<LogDouble> vf;

 public:
  LogDoubleFactorial() { vf.push_back(1.0); }

  void Adjust(unsigned n) {
    for (; vf.size() <= n;) vf.push_back(vf.back() * double(vf.size()));
  }

  LogDouble Get(unsigned n) {
    Adjust(n);
    return vf[n];
  }

  LogDouble operator()(unsigned n) { return Get(n); }

  LogDouble BinomialCoefficient(unsigned n, unsigned k) {
    if (k > n) return LogDouble();
    Adjust(n);
    return vf[n] / (vf[k] * vf[n - k]);
  }
};
}  // namespace numeric
