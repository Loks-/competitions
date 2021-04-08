#pragma once

#include "common/optimization/root/bisection.h"

namespace opt {
// Finding root on interval for continuous function.
template <class TFunction>
inline double Root(TFunction& f, double l, double r, double eps) {
  return root::Bisection(f, l, r, eps);
}
}  // namespace opt
