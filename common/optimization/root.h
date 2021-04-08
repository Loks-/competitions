#pragma once

#include "common/optimization/root/binary_search.h"

namespace opt {
// Finding root on interval for continuous function.
template <class TFunction>
inline double Root(TFunction& f, double l, double r, double eps) {
  return root::BinarySearch(f, l, r, eps);
}
}  // namespace opt
