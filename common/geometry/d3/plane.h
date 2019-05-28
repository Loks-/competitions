#pragma once

#include "common/base.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

namespace geometry {
namespace d3 {
template <class T>
class Plane {
 public:
  T a, b, c, d;

  Plane() : a(), b(), c(), d() {}
  Plane(const T& _a, const T& _b, const T& _c, const T& _d = T(0))
      : a(_a), b(_b), c(_c), d(_d) {}

  bool Valid() const { return (a != 0) || (b != 0) || (c != 0); }

  Vector<T> Normal() const { return Vector<T>(a, b, c); }

  T operator()(const Point<T>& p) const {
    return a * p.x + b * p.y + c * p.z + d;
  }

  void Flip() {
    a = -a;
    b = -b;
    c = -c;
    d = -d;
  }

  void Normalize() {
    assert(Valid());
    T l = Normal().Length();
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
