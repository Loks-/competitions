
#pragma once

#include "common/base.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/utils/center_of_mass.h"
#include "common/geometry/d2/utils/midpoint.h"
#include "common/geometry/d2/vector.h"

#include <algorithm>
#include <vector>
#include <utility>

// Find a line parallel to direction that split set of points into two halves.
template <class T>
constexpr geometry::d2::LinePV<T> HalfSplittingLineDL(
    const std::vector<geometry::d2::Point<T>>& points,
    const geometry::d2::Vector<T>& direction) {
  const unsigned n = points.size();
  assert(n > 0);
  const geometry::d2::LinePV<T> l0(CenterOfMass(points), direction);
  std::vector<std::pair<T, unsigned>> vp;
  vp.reserve(points.size());
  for (unsigned i = 0; i < n; ++i) vp.push_back({l0(points[i]), i});
  const unsigned m = (n - 1) / 2;
  std::nth_element(vp.begin(), vp.begin() + m, vp.end());
  if (n & 1) {
    return {points[vp[m].second], direction};
  } else {
    std::nth_element(vp.begin() + m, vp.begin() + m + 1, vp.end());
    return {MidPoint(points[vp[m].second], points[vp[m + 1].second]),
            direction};
  }
}
