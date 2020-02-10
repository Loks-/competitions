#pragma once

#include "common/base.h"
#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
// Rectangle with edges parallel to coordinate axis
template <class T>
class ARectagnle {
 public:
  Point<T> p1, p2;

  ARectagnle() : p1(), p2() {}
  ARectagnle(const Point<T>& _p1, const Point<T>& _p2) : p1(_p1), p2(_p2) {}

  bool Inside(const Point<T>& p) {
    return (p1.x <= p.x) && (p.x <= p2.x) && (p1.y <= p.y) && (p.y <= p2.y);
  }

  bool StrictInside(const Point<T>& p) {
    return (p1.x < p.x) && (p.x < p2.x) && (p1.y < p.y) && (p.y < p2.y);
  }
};
}  // namespace d2
}  // namespace geometry

using D2ARectagnle = geometry::d2::ARectagnle<double>;
using I2ARectagnle = geometry::d2::ARectagnle<int64_t>;
