#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

namespace geometry {
namespace d2 {
template <class T>
class Reflection {
 public:
  Point<T> p;
  Vector<T> v, v2;

  void UpdateV2() {
    assert(!v.Empty());
    v2 = v * T(2) / (v * v);
  }

  Reflection(const Point<T>& p1, const Point<T>& p2) : p(p1), v(p2 - p1) {
    UpdateV2();
  }

  Reflection(const Point<T>& _p, const Vector<T>& _v) : p(_p), v(_v) {
    UpdateV2();
  }

  Reflection() : Reflection(Point<T>(), Vector<T>(T(1), T(0))) {}

  Point<T> operator()(const Point<T>& pp) const {
    auto pv = pp - p;
    return p + v2 * (pv * v) - pv;
  }
};
}  // namespace d2
}  // namespace geometry

using D2Reflection = geometry::d2::Reflection<double>;
