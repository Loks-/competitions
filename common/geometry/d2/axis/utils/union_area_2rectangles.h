#pragma once

#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/axis/utils/intersection_area_2rectangles.h"

template <class T>
T UnionArea(const geometry::d2::axis::Rectangle<T>& r1,
            const geometry::d2::axis::Rectangle<T>& r2) {
  return r1.Area() + r2.Area() - IntersectionArea(r1, r2);
}
