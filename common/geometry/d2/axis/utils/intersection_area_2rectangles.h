#pragma once

#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/compare/under.h"
#include "common/geometry/d2/utils/dmax.h"
#include "common/geometry/d2/utils/dmin.h"

template <class T>
constexpr T IntersectionArea(const geometry::d2::axis::Rectangle<T>& r1,
                             const geometry::d2::axis::Rectangle<T>& r2) {
  auto p1 = DMax(r1.p1, r2.p1), p2 = DMin(r1.p2, r2.p2);
  if (!StrictUnder(p1, p2)) return {};
  return (p2.x - p1.x) * (p2.y - p1.y);
}
