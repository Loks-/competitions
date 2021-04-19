#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_xy.h"
#include "common/geometry/d2/iangle.h"
#include "common/geometry/d2/point.h"

#include <algorithm>
#include <utility>
#include <vector>

// Find a line that split set of points into two halves.
std::pair<unsigned, unsigned> HalfSplittingLineAB(
    const std::vector<I2Point>& points) {
  assert(points.size() >= 2);
  unsigned a = 0;
  for (unsigned i = 1; i < points.size(); ++i) {
    if (CompareXY(points[i], points[a])) a = i;
  }
  std::vector<std::pair<I2Angle, unsigned>> va;
  va.reserve(points.size() - 1);
  for (unsigned i = 0; i < points.size(); ++i) {
    if (i == a) continue;
    va.push_back({I2Angle(points[i] - points[a]), i});
  }
  unsigned m = va.size() / 2;
  std::nth_element(va.begin(), va.begin() + m, va.end());
  return {a, va[m].second};
}
