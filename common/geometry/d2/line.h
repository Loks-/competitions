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
  T a, b, c;

  Line() : a(), b(), c() {}
  Line(const T& _a, const T& _b, const T& _c = T(0)) : a(_a), b(_b), c(_c) {}

  Line(const Point<T>& p1, const Point<T>& p2) {
    auto v = p2 - p1;
    a = -v.dy;
    b = v.dx;
    c = -(a * p1.x + b * p1.y);
  }

  bool Valid() const { return (a != 0) || (b != 0); }
  Vector<T> Normal() const { return Vector<T>(a, b); }
  T operator()(const Point<T>& p) const { return a * p.x + b * p.y + c; }

  void SetOppositeNormal() {
    a = -a;
    b = -b;
    c = -c;
  }

  void Normalize() {
    assert(Valid());
    T l = Normal().Length();
    a /= l;
    b /= l;
    c /= l;
  }
};
}  // namespace d2
}  // namespace geometry

using D2Line = geometry::d2::Line<double>;
using I2Line = geometry::d2::Line<int64_t>;
