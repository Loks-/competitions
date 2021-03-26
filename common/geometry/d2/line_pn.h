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
  Point<T> p;
  Vector<T> n;

  LinePN() {}
  LinePN(const Point<T>& _p, const Vector<T>& _n) : p(_p), n(_n) {}

  LinePN(const Point<T>& p1, const Point<T>& p2) : p(p1) {
    auto v = p2 - p1;
    n = Vector<T>(-v.dy, v.dx);
  }

  bool Valid() const { return !n.Empty(); }
  const Vector<T>& Normal() const { return n; }
  T operator()(const Point<T>& pp) const { return n * (pp - p); }

  void SetOppositeNormal() { n = -n; }
  void Normalize() { n.Normalize(); }
};
}  // namespace d2
}  // namespace geometry

using D2LinePN = geometry::d2::LinePN<double>;
using I2LinePN = geometry::d2::LinePN<int64_t>;
