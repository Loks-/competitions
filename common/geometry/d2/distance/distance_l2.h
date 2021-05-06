#pragma once

#include "common/geometry/d2/point.h"

#include <cmath>

template <class T>
inline T SquaredDistanceL2(const geometry::d2::Point<T>& p1,
                           const geometry::d2::Point<T>& p2) {
  T dx = p2.x - p1.x, dy = p2.y - p1.y;
  return dx * dx + dy * dy;
}

template <class T>
inline T DistanceL2(const geometry::d2::Point<T>& p1,
                    const geometry::d2::Point<T>& p2) {
  return sqrt(SquaredDistanceL2(p1, p2));
}
