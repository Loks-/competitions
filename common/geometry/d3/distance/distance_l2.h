#pragma once

#include "common/geometry/d3/point.h"

#include <cmath>

template <class T>
constexpr T SquaredDistanceL2(const geometry::d3::Point<T>& p1,
                              const geometry::d3::Point<T>& p2) {
  T dx = p2.x - p1.x, dy = p2.y - p1.y, dz = p2.z - p1.z;
  return dx * dx + dy * dy + dz * dz;
}

template <class T>
constexpr T DistanceL2(const geometry::d3::Point<T>& p1,
                       const geometry::d3::Point<T>& p2) {
  return sqrt(SquaredDistanceL2(p1, p2));
}
