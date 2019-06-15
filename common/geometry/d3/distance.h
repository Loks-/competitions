#pragma once

#include "common/geometry/d3/distance/distance_l2.h"
#include "common/geometry/d3/point.h"

template <class T>
inline T Distance(const geometry::d3::Point<T>& p1,
                  const geometry::d3::Point<T>& p2) {
  return DistanceL2(p1, p2);
}
