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
  using TPoint = Point<T>;
  using TVector = Vector<T>;
  using TSelf = LinePV<T>;

 public:
  TPoint p;
  TVector v;

 public:
  constexpr LinePV() {}

  constexpr LinePV(const TPoint& _p, const TVector& _v) : p(_p), v(_v) {}

  constexpr LinePV(const TPoint& p1, const TPoint& p2) : p(p1), v(p2 - p1) {}

  constexpr bool Valid() const { return !v.Empty(); }

  constexpr TVector Normal() const { return v.RotateHalfPi(); }

  constexpr T operator()(const TPoint& pp) const { return v % (pp - p); }

  constexpr void SetOppositeNormal() { v = -v; }

  constexpr void Normalize() { v.Normalize(); }

  constexpr bool operator==(const TSelf& r) const {
    return (r(p) == 0) && ((v % r.v) == 0);
  }

  constexpr bool operator!=(const TSelf& r) const {
    return (r(p) != 0) || ((v % r.v) != 0);
  }
};
}  // namespace d2
}  // namespace geometry

using D2LinePV = geometry::d2::LinePV<double>;
using I2LinePV = geometry::d2::LinePV<int64_t>;
