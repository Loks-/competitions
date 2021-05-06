#pragma once

#include "common/geometry/d2/point.h"

template <class T>
inline geometry::d2::Point<T> MidPoint(const geometry::d2::Point<T>& p1,
                                       const geometry::d2::Point<T>& p2) {
  return geometry::d2::Point<T>((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}
