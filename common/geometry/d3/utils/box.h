#pragma once

#include "common/geometry/d3/axis/rectangle.h"
#include "common/geometry/d3/point.h"

#include <algorithm>
#include <vector>

template <class T>
inline geometry::d3::axis::Rectangle<T> Box(
    const std::vector<geometry::d3::Point<T>>& points) {
  if (points.size() == 0) return {};
  geometry::d3::axis::Rectangle<T> r(points[0], points[0]);
  for (auto& p : points) {
    r.p1.x = std::min(r.p1.x, p.x);
    r.p1.y = std::min(r.p1.y, p.y);
    r.p1.z = std::min(r.p1.z, p.z);
    r.p2.x = std::max(r.p2.x, p.x);
    r.p2.y = std::max(r.p2.y, p.y);
    r.p2.z = std::max(r.p2.z, p.z);
  }
  return r;
}
