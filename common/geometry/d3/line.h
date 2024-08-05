#pragma once

#include "common/base.h"
#include "common/geometry/d3/base.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

namespace geometry {
namespace d3 {
template <class T>
class Line {
 public:
  Point<T> p;
  Vector<T> v;

  constexpr Line() {}
  constexpr Line(const Point<T>& _p, const Vector<T>& _v) : p(_p), v(_v) {}

  constexpr Point<T> operator()(const T& t) const {
    return Point<T>(p.x + t * v.dx, p.y + t * v.dy, p.z + t * v.dz);
  }

  constexpr T Project(const Point<T>& x) const {
    return ((x - p) * v) / (v * v);
  }
};
}  // namespace d3
}  // namespace geometry

using D3Line = geometry::d3::Line<double>;
using I3Line = geometry::d3::Line<int64_t>;
