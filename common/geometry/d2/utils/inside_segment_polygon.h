#pragma once

#include "common/geometry/d2/angle_int.h"
#include "common/geometry/d2/location/location.h"
#include "common/geometry/d2/location/point_polygon.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/has_point_segment.h"
#include "common/geometry/d2/utils/inside_iangle.h"
#include "common/geometry/d2/utils/intersect_segment.h"

namespace geometry {
namespace d2 {
namespace hidden {
template <class T>
constexpr bool InsideI(const Segment<T, true>& s, const Polygon<T>& plgn,
                       const location::Location& l1,
                       const location::Location& l2) {
  if ((l1.type == location::Location::OUTSIDE) ||
      (l2.type == location::Location::OUTSIDE))
    return false;
  if (s.Empty()) return true;
  if (l1.type == location::Location::VERTEX) {
    // Check that direction is inside
    const auto p0 = s.p1;
    assert(plgn[l1.index] == p0);
    const auto p1 = plgn.MGet(l1.index + plgn.Size() - 1),
               p2 = plgn.MGet(l1.index + 1);
    const IAngle<T> as(s.p2 - p0), a1(p1 - p0), a2(p2 - p0);
    if (as == a1) return HasPoint(Segment<T, true>(p1, p0), s.p2);
    if (as == a2) return HasPoint(Segment<T, true>(p2, p0), s.p2);
    if (!Inside(a2, a1, as)) return false;
  }
  if (l2.type == location::Location::VERTEX) {
    // Check that direction is inside
    const auto p0 = s.p2;
    assert(plgn[l2.index] == p0);
    const auto p1 = plgn.MGet(l2.index + plgn.Size() - 1),
               p2 = plgn.MGet(l2.index + 1);
    const IAngle<T> as(s.p1 - p0), a1(p1 - p0), a2(p2 - p0);
    if (as == a1) return HasPoint(Segment<T, true>(p1, p0), s.p1);
    if (as == a2) return HasPoint(Segment<T, true>(p2, p0), s.p1);
    if (!Inside(a2, a1, as)) return false;
  }
  if (l1.type == location::Location::EDGE) {
    const auto pp1 = plgn[l1.index], pp2 = plgn.MGet(l1.index + 1);
    const auto t = (pp2 - pp1) % (s.p2 - s.p1);
    if (t < 0) return false;
    if (t == 0) return HasPoint(Segment<T, true>(pp1, pp2), s.p2);
  }
  if (l2.type == location::Location::EDGE) {
    const auto pp1 = plgn[l2.index], pp2 = plgn.MGet(l2.index + 1);
    const auto t = (pp2 - pp1) % (s.p1 - s.p2);
    if (t < 0) return false;
    if (t == 0) return HasPoint(Segment<T, true>(pp1, pp2), s.p1);
  }
  for (unsigned i = 0; i < plgn.Size(); ++i) {
    const Segment<T, false> s0(s.p1, s.p2), sp(plgn[i], plgn.MGet(i + 1));
    if (Intersect(s0, sp)) return false;
  }
  return true;
}

template <class T>
constexpr bool InsideI(const Segment<T, true>& s, const Polygon<T>& plgn) {
  return InsideI(s, plgn, location::Locate(s.p1, plgn),
                 location::Locate(s.p2, plgn));
}
}  // namespace hidden

template <class T>
constexpr bool Inside(const Segment<T, true>& s, const Polygon<T>& plgn) {
  for (const auto& p : plgn.Vertices()) {
    if ((p != s.p1) && (p != s.p2) && HasPoint(s, p))
      return Inside(Segment<T, true>(s.p1, p), plgn) &&
             Inside(Segment<T, true>(p, s.p2), plgn);
  }
  return hidden::InsideI(s, plgn);
}
}  // namespace d2
}  // namespace geometry
