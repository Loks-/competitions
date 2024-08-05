#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

namespace geometry {
namespace d2 {
// Similar to Line class but in point-vector form.
template <class T>
class LinePV {
 public:
  Point<T> p;
  Vector<T> v;

  constexpr LinePV() {}

  constexpr LinePV(const Point<T>& _p, const Vector<T>& _v) : p(_p), v(_v) {}

  constexpr LinePV(const Point<T>& p1, const Point<T>& p2)
      : p(p1), v(p2 - p1) {}

  constexpr bool Valid() const { return !v.Empty(); }

  constexpr Vector<T> Normal() const { return v.RotateHalfPi(); }

  constexpr T operator()(const Point<T>& pp) const { return v % (pp - p); }

  void SetOppositeNormal() { v = -v; }
  void Normalize() { v.Normalize(); }

  constexpr bool operator==(const LinePV<T>& r) const {
    return (r(p) == 0) && ((v % r.v) == 0);
  }

  constexpr bool operator!=(const LinePV<T>& r) const {
    return (r(p) != 0) || ((v % r.v) != 0);
  }
};
}  // namespace d2
}  // namespace geometry

using D2LinePV = geometry::d2::LinePV<double>;
using I2LinePV = geometry::d2::LinePV<int64_t>;
