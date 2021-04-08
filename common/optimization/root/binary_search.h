#pragma once

#include "common/base.h"

namespace opt {
namespace root {
// Finding root on interval for continuous function.
template <class TFunction>
inline double BinarySearch(TFunction& f, double l, double r, double eps) {
  auto fl = f(l), fr = f(r);
  if ((0 < fl) && (fr < 0)) {
    return BinarySearch([&](double x) { return -f(x); }, l, r, eps);
  }
  assert((fl <= 0) && (0 <= fr));
  for (; r - l > eps;) {
    double m = (l + r) / 2;
    if (f(m) <= 0) {
      l = m;
    } else {
      r = m;
    }
  }
  return (l + r) / 2;
}
}  // namespace root
}  // namespace opt
