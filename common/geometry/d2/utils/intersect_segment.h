#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/has_point_segment.h"

#include <algorithm>

template <class T, template <class TT> class TObject>
inline bool IntersectSegment(const geometry::d2::Segment<T, false>& s,
                             const TObject<T>& o) {
  T v1 = o(s.p1), v2 = o(s.p2);
  return ((v1 < 0) && (0 < v2)) || ((v2 < 0) && (0 < v1));
}

template <class T, template <class TT> class TObject>
inline bool IntersectSegment(const geometry::d2::Segment<T, true>& s,
                             const TObject<T>& o) {
  T v1 = o(s.p1), v2 = o(s.p2);
  return ((v1 <= 0) && (0 <= v2)) || ((v2 <= 0) && (0 <= v1));
}

template <class T, bool closed1, bool closed2>
inline bool Intersect(const geometry::d2::Segment<T, closed1>& s1,
                      const geometry::d2::Segment<T, closed2>& s2) {
  if (s1.Empty()) return closed1 && HasPoint(s2, s1.p1);
  if (s2.Empty()) return closed2 && HasPoint(s1, s2.p1);
  geometry::d2::LinePV<T> l1(s1.p1, s1.p2), l2(s2.p1, s2.p2);
  if (l1 != l2) return IntersectSegment(s1, l2) && IntersectSegment(s2, l1);
  auto v0 = l1.v, v1 = s2.p1 - s1.p1, v2 = s2.p2 - s1.p1;
  auto t0 = v0 * v0, t1 = v1 * v0, t2 = v2 * v0;
  if (t2 < t1) std::swap(t1, t2);
  if (closed1 && closed2)
    return (0 <= t2) && (t1 <= t0);
  else
    return (0 < t2) && (t1 < t0);
}
