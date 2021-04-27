#pragma once

#include "common/geometry/d2/point.h"
#include "common/geometry/d2/utils/rotate_pi4s_point.h"

#include <vector>

template <class T>
inline std::vector<geometry::d2::Point<T>>& RotatePi4S(
    std::vector<geometry::d2::Point<T>>& points) {
  for (auto& p : points) p = RotatePi4S(p);
  return points;
}
