#pragma once

#include "common/geometry/d2/convex_hull/graham_scan_wcp.h"
#include "common/geometry/d2/convex_polygon.h"
#include "common/geometry/d2/point.h"

#include <vector>

template <class T>
// Without collinear points
inline geometry::d2::ConvexPolygon<T> ConvexHullWCP(
    const std::vector<geometry::d2::Point<T>>& vpoints) {
  return geometry::d2::convex_hull::GrahamScanWCP(vpoints);
}
