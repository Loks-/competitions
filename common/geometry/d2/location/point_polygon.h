#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/location/location.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/has_point_segment.h"
#include "common/geometry/d2/utils/intersect_segment.h"

namespace geometry {
namespace d2 {
namespace location {
inline Location Locate(const I2Point& p, const I2Polygon& plgn) {
  unsigned cnt = 0;
  if (p == plgn[0]) return {Location::VERTEX, 0};
  I2OpenSegment s0(p, {p.x, plgn[0].y - 1});
  for (unsigned i = 0; i < plgn.Size(); ++i) {
    auto p1 = plgn[i], p2 = plgn.MGet(i + 1);
    if (p2 == p) return {Location::VERTEX, i + 1};
    I2OpenSegment s(p1, p2);
    if (HasPoint(s, p)) return {Location::EDGE, i};
    if (Intersect(s0, s)) ++cnt;
    if ((p1.x == p.x) && (p1.y < p.y) && (p2.x < p.x)) ++cnt;
    if ((p2.x == p.x) && (p1.y < p.y) && (p1.x < p.x)) ++cnt;
  }
  return {(cnt % 2) ? Location::INSIDE : Location::OUTSIDE, 0};
}
}  // namespace location
}  // namespace d2
}  // namespace geometry
