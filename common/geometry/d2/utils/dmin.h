#pragma once

#include "common/geometry/d2/point.h"

#include <algorithm>

template <class T>
inline geometry::d2::Point<T> DMin(const geometry::d2::Point<T>& p1,
                                   const geometry::d2::Point<T>& p2) {
  return {std::min(p1.x, p2.x), std::min(p1.y, p2.y)};
}
