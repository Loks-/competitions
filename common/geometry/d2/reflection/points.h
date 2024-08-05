#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
namespace reflection {
template <class T>
constexpr bool Reflected(const LinePV<T>& line, const Point<T>& p1,
                         const Point<T>& p2) {
  return ((line.v * (p2 - p1)) == T(0)) &&
         (SquaredDistanceL2(line.p, p1) == SquaredDistanceL2(line.p, p2));
}
}  // namespace reflection
}  // namespace d2
}  // namespace geometry
