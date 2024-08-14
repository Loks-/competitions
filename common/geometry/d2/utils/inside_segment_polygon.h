#pragma once

#include "common/geometry/d2/angle_int.h"
#include "common/geometry/d2/location.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/has_point_segment.h"
#include "common/geometry/d2/utils/inside_iangle.h"
#include "common/geometry/d2/utils/intersect_segment.h"
#include "common/geometry/d2/utils/locate_point_polygon.h"

namespace geometry {
namespace d2 {
template <class T>
class InisideSegmentPolygon {
 public:
  using TPolygon = Polygon<T>;
  using TCSegment = Segment<T, true>;
  using TOSegment = Segment<T, false>;

 public:
  // Assume that polygon verices are not inside the segment.
  static constexpr bool InsideI(const TCSegment& s, const TPolygon& plgn,
                                const Location& l1, const Location& l2) {
    if ((l1.type == Location::OUTSIDE) || (l2.type == Location::OUTSIDE))
      return false;
    if (s.Empty()) return true;
    if (l1.type == Location::VERTEX) {
      // Check that direction is inside
      const auto p0 = s.p1;
      assert(plgn[l1.index] == p0);
      const auto p1 = plgn.MGet(l1.index + plgn.Size() - 1),
                 p2 = plgn.MGet(l1.index + 1);
      const IAngle<T> as(s.p2 - p0), a1(p1 - p0), a2(p2 - p0);
      if (as == a1) return HasPoint(TCSegment(p1, p0), s.p2);
      if (as == a2) return HasPoint(TCSegment(p2, p0), s.p2);
      if (!geometry::d2::Inside(a2, a1, as)) return false;
    }
    if (l2.type == Location::VERTEX) {
      // Check that direction is inside
      const auto p0 = s.p2;
      assert(plgn[l2.index] == p0);
      const auto p1 = plgn.MGet(l2.index + plgn.Size() - 1),
                 p2 = plgn.MGet(l2.index + 1);
      const IAngle<T> as(s.p1 - p0), a1(p1 - p0), a2(p2 - p0);
      if (as == a1) return HasPoint(TCSegment(p1, p0), s.p1);
      if (as == a2) return HasPoint(TCSegment(p2, p0), s.p1);
      if (!geometry::d2::Inside(a2, a1, as)) return false;
    }
    if (l1.type == Location::EDGE) {
      const auto pp1 = plgn[l1.index], pp2 = plgn.MGet(l1.index + 1);
      const auto t = (pp2 - pp1) % (s.p2 - s.p1);
      if (t < 0) return false;
      if (t == 0) return HasPoint(TCSegment(pp1, pp2), s.p2);
    }
    if (l2.type == Location::EDGE) {
      const auto pp1 = plgn[l2.index], pp2 = plgn.MGet(l2.index + 1);
      const auto t = (pp2 - pp1) % (s.p1 - s.p2);
      if (t < 0) return false;
      if (t == 0) return HasPoint(TCSegment(pp1, pp2), s.p1);
    }
    for (unsigned i = 0; i < plgn.Size(); ++i) {
      const TOSegment s0(s.p1, s.p2), sp(plgn[i], plgn.MGet(i + 1));
      if (Intersect(s0, sp)) return false;
    }
    return true;
  }

  // Assume that polygon verices are not inside the segment.
  static constexpr bool InsideI(const TCSegment& s, const TPolygon& plgn) {
    return InsideI(s, plgn, Locate(s.p1, plgn), Locate(s.p2, plgn));
  }

  // Check if some polygon vertices are inside the segment.
  static constexpr bool Inside(const TCSegment& s, const TPolygon& plgn) {
    for (const auto& p : plgn.Vertices()) {
      if ((p != s.p1) && (p != s.p2) && HasPoint(s, p))
        return Inside(TCSegment(s.p1, p), plgn) &&
               Inside(TCSegment(p, s.p2), plgn);
    }
    return InsideI(s, plgn);
  }

  static constexpr bool Inside(const TOSegment& s, const TPolygon& plgn) {
    return (s.Empty() ? true : Inside(TCSegment{s.p1, s.p2}, plgn));
  }
};

template <class T, bool closed>
constexpr bool Inside(const Segment<T, closed>& s, const Polygon<T>& plgn) {
  return InisideSegmentPolygon<T>::Inside(s, plgn);
}
}  // namespace d2
}  // namespace geometry
