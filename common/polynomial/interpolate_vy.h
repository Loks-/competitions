#pragma once

#include "common/polynomial/base_newton_polynomial.h"
#include <vector>

template <class TValue>
inline TValue Interpolate(const std::vector<TValue>& vy, const TValue& x) {
  return BaseNewtonPolynomial<TValue>().Interpolate(vy)(x);
}
