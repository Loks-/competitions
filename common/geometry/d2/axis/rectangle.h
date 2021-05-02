#pragma once

#include "common/base.h"
#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
namespace axis {
// Rectangle with edges parallel to coordinate axis
template <class T>
class Rectagnle {
 public:
  Point<T> p1, p2;

  Rectagnle() : p1(), p2() {}
  Rectagnle(const Point<T>& _p1, const Point<T>& _p2) : p1(_p1), p2(_p2) {}

  bool Inside(const Point<T>& p) const {
    return (p1.x <= p.x) && (p.x <= p2.x) && (p1.y <= p.y) && (p.y <= p2.y);
  }

  bool StrictInside(const Point<T>& p) const {
    return (p1.x < p.x) && (p.x < p2.x) && (p1.y < p.y) && (p.y < p2.y);
  }
};
}  // namespace axis
}  // namespace d2
}  // namespace geometry

using D2ARectagnle = geometry::d2::axis::Rectagnle<double>;
using I2ARectagnle = geometry::d2::axis::Rectagnle<int64_t>;
