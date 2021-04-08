#pragma once

#include <cmath>

namespace opt {
namespace minimum {
template <class TFunction>
inline double GoldenSectionSearch(TFunction& f, double l, double r,
                                  double eps) {
  thread_local double invphi = (sqrt(5.0) - 1) / 2.0, invphi2 = invphi * invphi;
  double d = r - l, l1 = l + d * invphi2, r1 = l + d * invphi,
         eps2 = eps / invphi;
  if (d <= eps) return (l + r) / 2;
  auto fl1 = f(l1), fr1 = f(r1);
  for (; d > eps2;) {
    if (fl1 < fr1) {
      r = r1;
      r1 = l1;
      fr1 = fl1;
      d = r - l;
      l1 = l + d * invphi2;
      fl1 = f(l1);
    } else {
      l = l1;
      l1 = r1;
      fl1 = fr1;
      d = r - l;
      r1 = l + d * invphi;
      fr1 = f(r1);
    }
  }
  return (fl1 < fr1) ? (l + r1) / 2 : (l1 + r) / 2;
}
}  // namespace minimum
}  // namespace opt
