#pragma once

#include "common/geometry/d2/point.h"
#include "common/optimization/minimum.h"

namespace opt {
// Local minimum for continuous function.
// Global minimum for unimodal function.
template <class TFunction>
inline D2Point Minimum2D(TFunction& f, double xl, double xr, double yl,
                         double yr, double eps) {
  double cy;
  auto fx = [&](double x) { return f(x, cy); };
  auto fy = [&](double y) {
    cy = y;
    double x = Minimum(fx, xl, xr, eps);
    return f(x, y);
  };
  double ry = Minimum(fy, yl, yr, eps);
  cy = ry;
  double rx = Minimum(fx, xl, xr, eps);
  return {rx, ry};
}

template <class TFunction>
inline D2Point Minimum2DSafe(TFunction& f, double xl, double xr, double yl,
                             double yr, double eps) {
  eps /= 2.0;
  auto fyx = [&](double y, double x) { return f(x, y); };
  auto p1 = Minimum2D(f, xl, xr, yl, yr, eps),
       p2 = Minimum2D(fyx, yl, yr, xl, xr, eps);
  std::swap(p2.x, p2.y);
  return f(p1.x, p1.y) < f(p2.y, p2.x) ? p1 : D2Point(p2.y, p2.x);
}
}  // namespace opt
