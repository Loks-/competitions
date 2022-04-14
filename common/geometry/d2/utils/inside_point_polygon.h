#pragma once

#include "common/geometry/d2/location/location.h"
#include "common/geometry/d2/location/point_polygon.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"

namespace geometry {
namespace d2 {
template <class T>
inline bool Inside(const Point<T>& p, const Polygon<T>& plgn) {
  return location::Locate(p, plgn).type != location::Location::OUTSIDE;
}
}  // namespace d2
}  // namespace geometry
