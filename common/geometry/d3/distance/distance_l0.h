#pragma once

#include "common/geometry/d3/point.h"
#include "common/numeric/utils/abs.h"

#include <algorithm>

inline int64_t DistanceL0(const geometry::d3::Point<int64_t>& p1,
                          const geometry::d3::Point<int64_t>& p2) {
  return std::max(Abs(p1.x - p2.x),
                  std::max(Abs(p1.y - p2.y), Abs(p1.z - p2.z)));
}
