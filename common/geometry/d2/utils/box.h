#pragma once

#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/point.h"

#include <algorithm>
#include <vector>

template <class T>
inline geometry::d2::axis::Rectangle<T> Box(
    const std::vector<geometry::d2::Point<T>>& points) {
  if (points.size() == 0) return {};
  geometry::d2::axis::Rectangle<T> r(points[0], points[0]);
  for (auto& p : points) {
    r.p1.x = std::min(r.p1.x, p.x);
    r.p1.y = std::min(r.p1.y, p.y);
    r.p2.x = std::max(r.p2.x, p.x);
    r.p2.y = std::max(r.p2.y, p.y);
  }
  return r;
}
