#pragma once

#include "common/polynomial/base_newton.h"
#include <vector>

namespace polynomial {
template <class TValue>
inline TValue Interpolate(const std::vector<TValue>& vy, const TValue& x) {
  return BaseNewton<TValue>().Interpolate(vy)(x);
}
}  // namespace polynomial
