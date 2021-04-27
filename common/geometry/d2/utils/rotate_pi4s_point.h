#pragma once

#include "common/geometry/d2/point.h"

template <class T>
inline geometry::d2::Point<T> RotatePi4S(const geometry::d2::Point<T>& p) {
  return {p.x - p.y, p.x + p.y};
}
