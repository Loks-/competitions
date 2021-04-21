
#pragma once

#include "common/base.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/utils/center_of_mass.h"
#include "common/geometry/d2/utils/midpoint.h"
#include "common/geometry/d2/vector.h"

#include <algorithm>
#include <vector>

// Find a line parallel to direction that split set of points into two halves.
template <class T>
inline geometry::d2::LinePV<T> HalfSplittingLineDL(
    const std::vector<geometry::d2::Point<T>>& points,
    const geometry::d2::Vector<T>& direction) {
  struct VP {
    T v;
    geometry::d2::Point<T> p;

    bool operator<(const VP& r) const { return v < r.v; }
  };

  unsigned n = points.size();
  assert(n > 0);
  geometry::d2::LinePV<T> l0(CenterOfMass(points), direction);
  std::vector<VP> vp;
  vp.reserve(points.size());
  for (auto& p : points) vp.push_back({l0(p), p});
  unsigned m = (n - 1) / 2;
  std::nth_element(vp.begin(), vp.begin() + m, vp.end());
  if (n & 1) {
    return {vp[m].p, direction};
  } else {
    std::nth_element(vp.begin() + m, vp.begin() + m + 1, vp.end());
    return {MidPoint(vp[m].p, vp[m + 1].p), direction};
  }
}
