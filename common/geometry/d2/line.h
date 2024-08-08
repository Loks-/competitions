#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

namespace geometry {
namespace d2 {
// Line in linear equation form.
template <class T>
class Line {
 public:
  using TPoint = Point<T>;
  using TVector = Vector<T>;

 public:
  T a, b, c;

 public:
  constexpr Line() : a(), b(), c() {}

  constexpr Line(const T& _a, const T& _b, const T& _c = T(0))
      : a(_a), b(_b), c(_c) {}

  constexpr Line(const TPoint& p1, const TPoint& p2) {
    const auto v = p2 - p1;
    a = -v.dy;
    b = v.dx;
    c = -(a * p1.x + b * p1.y);
  }

  constexpr bool Valid() const { return (a != 0) || (b != 0); }

  constexpr TVector Normal() const { return TVector(a, b); }

  constexpr T operator()(const TPoint& p) const {
    return a * p.x + b * p.y + c;
  }

  constexpr void SetOppositeNormal() {
    a = -a;
    b = -b;
    c = -c;
  }

  constexpr void Normalize() {
    assert(Valid());
    const T l = Normal().Length();
    a /= l;
    b /= l;
    c /= l;
  }
};
}  // namespace d2
}  // namespace geometry

using D2Line = geometry::d2::Line<double>;
using I2Line = geometry::d2::Line<int64_t>;
