#pragma once

#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/point.h"

template <class T>
inline T SquaredDistance(const geometry::d2::Point<T>& p1,
                         const geometry::d2::Point<T>& p2) {
  return SquaredDistanceL2(p1, p2);
}

template <class T>
inline T Distance(const geometry::d2::Point<T>& p1,
                  const geometry::d2::Point<T>& p2) {
  return DistanceL2(p1, p2);
}
