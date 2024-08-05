#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"

template <class T>
constexpr bool Collinear(const geometry::d2::Point<T>& p1,
                         const geometry::d2::Point<T>& p2,
                         const geometry::d2::Point<T>& p3) {
  return (p2 - p1) % (p3 - p1) == T();
}
