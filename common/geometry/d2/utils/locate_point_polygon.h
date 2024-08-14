#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/location.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/has_point_segment.h"
#include "common/geometry/d2/utils/intersect_segment.h"

namespace geometry {
namespace d2 {
template <class T>
constexpr Location Locate(const Point<T>& p, const Polygon<T>& plgn) {
  unsigned cnt = 0;
  if (p == plgn[0]) return {Location::VERTEX, 0};
  const Segment<T, false> s0(p, {p.x, plgn[0].y - T(1)});
  for (unsigned i = 0; i < plgn.Size(); ++i) {
    const auto p1 = plgn[i], p2 = plgn.MGet(i + 1);
    if (p2 == p) return {Location::VERTEX, i + 1};
    const Segment<T, false> s(p1, p2);
    if (HasPoint(s, p)) return {Location::EDGE, i};
    if ((p1.x == p.x) || (p2.x == p.x)) {
      if ((p1.x == p.x) && (p1.y < p.y) && (p2.x < p.x)) ++cnt;
      if ((p2.x == p.x) && (p2.y < p.y) && (p1.x < p.x)) ++cnt;
    } else if (Intersect(s0, s)) {
      ++cnt;
    }
  }
  return {(cnt % 2) ? Location::INSIDE : Location::OUTSIDE, 0};
}
}  // namespace d2
}  // namespace geometry
