#pragma once

#include "common/geometry/d3/base.h"
#include "common/geometry/d3/plane_pn.h"
#include "common/geometry/d3/point.h"

template <class T>
constexpr geometry::d3::PlanePN<T> PlanePNFromPoints(
    const geometry::d3::Point<T>& p1, const geometry::d3::Point<T>& p2,
    const geometry::d3::Point<T>& p3) {
  auto v2 = p2 - p1, v3 = p3 - p1;
  T a = v2.dy * v3.dz - v2.dz * v3.dy;
  T b = v2.dz * v3.dx - v2.dx * v3.dz;
  T c = v2.dx * v3.dy - v2.dy * v3.dx;
  return geometry::d3::PlanePN<T>(p1, {a, b, c});
}
