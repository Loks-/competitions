#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

namespace geometry {
namespace d2 {
template <class T>
class Reflection {
 public:
  using TPoint = Point<T>;
  using TVector = Vector<T>;

 protected:
  TPoint p;
  TVector v, v2;

 protected:
  constexpr void UpdateV2() {
    assert(!v.Empty());
    v2 = v * T(2) / (v * v);
  }

 public:
  constexpr Reflection(const TPoint& p1, const TPoint& p2) : p(p1), v(p2 - p1) {
    UpdateV2();
  }

  constexpr Reflection(const TPoint& _p, const TVector& _v) : p(_p), v(_v) {
    UpdateV2();
  }

  constexpr Reflection() : Reflection(TPoint(), TVector(T(1), T(0))) {}

  constexpr TPoint operator()(const TPoint& pp) const {
    const auto pv = pp - p;
    return p + v2 * (pv * v) - pv;
  }
};
}  // namespace d2
}  // namespace geometry

using D2Reflection = geometry::d2::Reflection<double>;
