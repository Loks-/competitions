
#pragma once

#include "common/base.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/utils/midpoint.h"
#include "common/geometry/d2/vector.h"

#include <algorithm>
#include <vector>

// Find a line parallel to direction that split set of points into two halves.
inline D2LinePV HalfSplittingLineDL(const std::vector<D2Point>& points,
                                    const D2Vector& direction) {
  struct VP {
    double v;
    D2Point p;

    bool operator<(const VP& r) const { return v < r.v; }
  };

  unsigned n = points.size();
  assert(n > 0);
  D2LinePV l0(D2Point(), direction);
  std::vector<VP> vp;
  vp.reserve(points.size());
  for (auto& p : points) vp.push_back({l0(p), p});
  unsigned m = (n - 1) / 2;
  std::nth_element(vp.begin(), vp.begin() + m, vp.end());
  if (n & 1) {
    return D2LinePV(vp[m].p, direction);
  } else {
    std::nth_element(vp.begin() + m, vp.begin() + m + 1, vp.end());
    return D2LinePV(MidPoint(vp[m].p, vp[m + 1].p), direction);
  }
}
