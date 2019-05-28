#pragma once

#include "common/geometry/d3/point.h"

template <class T>
inline T Determinant(const geometry::d3::Point<T>& p1,
                     const geometry::d3::Point<T>& p2,
                     const geometry::d3::Point<T>& p3) {
  return p1.x * (p2.y * p3.z - p2.z * p3.y) +
         p1.y * (p2.z * p3.x - p2.x * p3.z) +
         p1.z * (p2.x * p3.y - p2.y * p3.x);
}
