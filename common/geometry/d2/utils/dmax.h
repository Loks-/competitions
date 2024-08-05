#pragma once

#include "common/geometry/d2/point.h"

#include <algorithm>

template <class T>
constexpr geometry::d2::Point<T> DMax(const geometry::d2::Point<T>& p1,
                                      const geometry::d2::Point<T>& p2) {
  return {std::max(p1.x, p2.x), std::max(p1.y, p2.y)};
}
