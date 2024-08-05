#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/segment.h"

template <class T, bool closed>
constexpr bool HasPoint(const geometry::d2::Segment<T, closed>& s,
                        const geometry::d2::Point<T>& p) {
  if ((p == s.p1) || (p == s.p2)) return closed;
  if (s.Empty()) return false;
  auto vs = s.p2 - s.p1, vp = p - s.p1;
  if (vs % vp != 0) return false;
  auto x = vs * vp;
  return (x > 0) && (x < vs * vs);
}
