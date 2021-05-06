#pragma once

#include "common/geometry/d2/point.h"

template <class T>
inline bool CompareXY(const geometry::d2::Point<T>& p1,
                      const geometry::d2::Point<T>& p2) {
  return (p1.x == p2.x) ? (p1.y < p2.y) : (p1.x < p2.x);
}
