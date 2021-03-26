#pragma once

namespace opt {
template <class TFunction>
inline double TernarySearch(TFunction& f, double l, double r, double eps) {
  for (; r - l > eps;) {
    double m1 = (2. * l + r) / 3., m2 = (l + 2. * r) / 3;
    if (f(m1) < f(m2))
      r = m2;
    else
      l = m1;
  }
  return (l + r) / 2;
}
}  // namespace opt
