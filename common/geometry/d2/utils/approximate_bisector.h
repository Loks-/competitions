#pragma once

#include "common/base.h"
#include "common/geometry/d2/vector.h"
#include "common/numeric/utils/gcd.h"

#include <algorithm>

// Vectors weight ratio is less than sqrt(2):1
template <class T>
inline geometry::d2::Vector<T> ApproximateBisector(geometry::d2::Vector<T> v1,
                                                   geometry::d2::Vector<T> v2) {
  auto l1 = v1.LengthSquared(), l2 = v2.LengthSquared();
  if (l1 > l2) {
    std::swap(l1, l2);
    std::swap(v1, v2);
  }
  assert(l1 > 0);
  for (; (l1 << 1) < l2; l1 <<= 2) v1 *= T(2);
  auto v = v1 + v2;
  auto g = GCDSafe(v.dx, v.dy);
  return v / g;
}
