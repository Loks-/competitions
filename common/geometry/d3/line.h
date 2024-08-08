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
  using TPoint = Point<T>;
  using TVector = Vector<T>;

 public:
  TPoint p;
  TVector v;

 public:
  constexpr Line() {}

  constexpr Line(const TPoint& _p, const TVector& _v) : p(_p), v(_v) {}

  constexpr TPoint operator()(const T& t) const {
    return TPoint(p.x + t * v.dx, p.y + t * v.dy, p.z + t * v.dz);
  }

  constexpr T Project(const TPoint& x) const { return ((x - p) * v) / (v * v); }
};
}  // namespace d3
}  // namespace geometry

using D3Line = geometry::d3::Line<double>;
using I3Line = geometry::d3::Line<int64_t>;
