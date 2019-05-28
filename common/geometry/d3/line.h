#pragma once

#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

namespace geometry {
namespace d3 {
template <class T>
class Line {
 public:
  Point<T> p;
  Vector<T> v;

  Line() {}
  Line(const Point<T>& _p, const Vector<T>& _v) : p(_p), v(_v) {}

  Point<T> operator()(const T& t) const {
    return Point<T>(p.x + t * v.dx, p.y + t * v.dy, p.z + t * v.dz);
  }
};
}  // namespace d3
}  // namespace geometry

using D3Line = geometry::d3::Line<double>;
