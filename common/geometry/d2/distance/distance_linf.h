#pragma once

#include "common/geometry/d2/point.h"
#include "common/numeric/utils/abs.h"

#include <algorithm>

inline int64_t DistanceLInf(const geometry::d2::Point<int64_t>& p1,
                            const geometry::d2::Point<int64_t>& p2) {
  return std::max(Abs(p1.x - p2.x), Abs(p1.y - p2.y));
}
