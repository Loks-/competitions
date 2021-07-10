#pragma once

#include "common/geometry/d2/angle_int.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/location/location.h"
#include "common/geometry/d2/location/point_polygon.h"
#include "common/geometry/d2/utils/inside_iangle.h"
#include "common/geometry/d2/utils/intersect_segment.h"

namespace geometry {
namespace d2 {
inline bool Inside(const I2ClosedSegment& s, const I2Polygon& plgn, 
                   const location::Location& l1, const location::Location& l2) {
  if ((l1.type == location::Location::OUTSIDE) || (l2.type == location::Location::OUTSIDE)) return false;
  if (l1.type == location::Location::VERTEX) {
    // Check that direction is inside
    auto p0 = s.p1;
    assert(plgn[l1.index] == p0);
    auto p1 = plgn.MGet(l1.index + plgn.Size() - 1),
         p2 = plgn.MGet(l1.index + 1);
    I2Angle as(s.p2 - p0), a1(p1 - p0), a2(p2 - p0);
    if (!Inside(a2, a1, as)) return false;
  }
  if (l2.type == location::Location::VERTEX) {
    // Check that direction is inside
    auto p0 = s.p2;
    assert(plgn[l2.index] == p0);
    auto p1 = plgn.MGet(l2.index + plgn.Size() - 1),
         p2 = plgn.MGet(l2.index + 1);
    I2Angle as(s.p1 - p0), a1(p1 - p0), a2(p2 - p0);
    if (!Inside(a2, a1, as)) return false;
  }
  if (l1.type == location::Location::EDGE) {
    if (((plgn.MGet(l1.index + 1) - plgn[l1.index]) % (s.p2 - s.p1)) < 0) return false;
  }
  if (l2.type == location::Location::EDGE) {
    if (((plgn.MGet(l2.index + 1) - plgn[l2.index]) % (s.p1 - s.p2)) < 0) return false;
  }
  for (unsigned i = 0; i < plgn.Size(); ++i) {
    I2OpenSegment sp(plgn[i], plgn.MGet(i + 1));
    if (Intersect(s, sp)) return false;
  }
  return true;
}

inline bool Inside(const I2ClosedSegment& s, const I2Polygon& plgn) {
  return Inside(s, plgn, location::Locate(s.p1, plgn), location::Locate(s.p2, plgn));
}
}  // namespace d2
}  // namespace geometry
