#pragma once

#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/location/location.h"
#include "common/geometry/d2/location/point_polygon.h"

namespace geometry {
namespace d2 {
inline bool Inside(const I2Point& p, const I2Polygon& plgn) {
  return location::Locate(p, plgn).type != location::Location::OUTSIDE;
}
}  // namespace d2
}  // namespace geometry
