#pragma once

#include "common/polynomial/base_newton.h"
#include <vector>

namespace polynomial {
template <class TValue>
inline BaseNewton<TValue> GetSumOfPowers(unsigned power) {
  std::vector<TValue> vy(power + 2, 0);
  for (unsigned i = 1; i < vy.size(); ++i)
    vy[i] = vy[i - 1] + TValue(i).PowU(power);
  BaseNewton<TValue> r;
  r.Interpolate(vy);
  return r;
}
}  // namespace polynomial
