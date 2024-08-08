#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

namespace geometry {
namespace d2 {
// Similar to Line class but in point-normal form.
template <class T>
class LinePN {
 public:
  using TPoint = Point<T>;
  using TVector = Vector<T>;
  using TSelf = LinePN<T>;

 public:
  TPoint p;
  TVector n;

 public:
  constexpr LinePN() {}

  constexpr LinePN(const TPoint& _p, const TVector& _n) : p(_p), n(_n) {}

  constexpr LinePN(const TPoint& p1, const TPoint& p2) : p(p1) {
    const auto v = p2 - p1;
    n = TVector(-v.dy, v.dx);
  }

  constexpr bool Valid() const { return !n.Empty(); }

  constexpr const TVector& Normal() const { return n; }

  constexpr T operator()(const TPoint& pp) const { return n * (pp - p); }

  constexpr void SetOppositeNormal() { n = -n; }

  constexpr void Normalize() { n.Normalize(); }
};
}  // namespace d2
}  // namespace geometry

using D2LinePN = geometry::d2::LinePN<double>;
using I2LinePN = geometry::d2::LinePN<int64_t>;
