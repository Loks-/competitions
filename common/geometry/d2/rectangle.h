#pragma once

#include "common/base.h"
#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
template <class T>
class Rectagnle {
 public:
  Point<T> p1, p2;

  Rectagnle() : p1(), p2() {}
  Rectagnle(const Point<T>& _p1, const Point<T>& _p2) : p1(_p1), p2(_p2) {}

  bool StrictInside(const Point<T>& p) {
    return (p1.x < p.x) && (p.x < p2.x) && (p1.y < p.y) && (p.y < p2.y);
  }
};
}  // namespace d2
}  // namespace geometry

using D2Rectagnle = geometry::d2::Rectagnle<double>;
using I2Rectagnle = geometry::d2::Rectagnle<int64_t>;
