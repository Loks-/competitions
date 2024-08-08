#pragma once

#include "common/base.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

namespace geometry {
namespace d3 {
// Plane in general form.
template <class T>
class Plane {
 public:
  using TPoint = Point<T>;
  using TVector = Vector<T>;

 public:
  T a, b, c, d;

 public:
  constexpr Plane() : a(), b(), c(), d() {}

  constexpr Plane(const T& _a, const T& _b, const T& _c, const T& _d = T(0))
      : a(_a), b(_b), c(_c), d(_d) {}

  constexpr bool Valid() const { return (a != 0) || (b != 0) || (c != 0); }

  constexpr TVector Normal() const { return {a, b, c}; }

  constexpr T operator()(const TPoint& p) const {
    return a * p.x + b * p.y + c * p.z + d;
  }

  constexpr void SetOppositeNormal() {
    a = -a;
    b = -b;
    c = -c;
    d = -d;
  }

  constexpr void Normalize() {
    assert(Valid());
    const T l = Normal().Length();
    a /= l;
    b /= l;
    c /= l;
    d /= l;
  }
};
}  // namespace d3
}  // namespace geometry

using D3Plane = geometry::d3::Plane<double>;
using I3Plane = geometry::d3::Plane<int64_t>;
