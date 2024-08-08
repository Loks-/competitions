#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/circle_int.h"
#include "common/geometry/d2/segment.h"

// Support coordinates up to 2^14 ~ 16k.
template <class T>
constexpr bool Intersect(const geometry::d2::Segment<T, true>& s,
                         const geometry::d2::ICircle<T>& c) {
  const auto r1 = c(s.p1), r2 = c(s.p2);
  if ((r1 > 0) && (r2 > 0)) {
    const auto v1 = s.p2 - s.p1, v2 = c.c - s.p1;
    const auto a = v1 * v1, b = v1 * v2, d = v2 * v2;
    if ((b <= 0) || (a <= b)) return false;
    return a * (d - c.r2) <= b * b;
  } else {
    return (r1 >= 0) || (r2 >= 0);
  }
}
