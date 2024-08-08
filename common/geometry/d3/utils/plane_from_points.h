#pragma once

#include "common/geometry/d3/base.h"
#include "common/geometry/d3/plane.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/utils/determinant.h"

template <class T>
constexpr geometry::d3::Plane<T> PlaneFromPoints(
    const geometry::d3::Point<T>& p1, const geometry::d3::Point<T>& p2,
    const geometry::d3::Point<T>& p3) {
  const auto v2 = p2 - p1, v3 = p3 - p1;
  const T d = -Determinant(p1, p2, p3);
  const T a = v2.dy * v3.dz - v2.dz * v3.dy;
  const T b = v2.dz * v3.dx - v2.dx * v3.dz;
  const T c = v2.dx * v3.dy - v2.dy * v3.dx;
  return geometry::d3::Plane<T>(a, b, c, d);
}
