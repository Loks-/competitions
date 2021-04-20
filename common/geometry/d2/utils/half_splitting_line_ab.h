#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_xy.h"
#include "common/geometry/d2/iangle.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"
#include "common/stl/pair.h"

#include <algorithm>
#include <utility>
#include <vector>

// Find a line that split set of points into two halves.
inline std::pair<unsigned, unsigned> HalfSplittingLineAB(
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

inline unsigned HalfSplittingLine0B(const std::vector<I2Point>& points) {
  unsigned n = points.size(), h = n / 2 + 1;
  assert(n >= 1);
  std::vector<std::pair<I2Angle, unsigned>> va;
  va.reserve(2 * n);
  for (unsigned i = 0; i < n; ++i)
    va.push_back({I2Angle(I2Vector(points[i])), i});
  std::sort(va.begin(), va.end());
  if (va.back() == va[0]) return 0;
  for (unsigned i = 0; i < n; ++i) va.push_back(va[i]);
  unsigned i = 0, j1 = 0, j2 = 0;
  auto AdjustJ = [&]() {
    auto a = va[i].first.ShiftPi();
    for (; (j1 < n) && (va[i] == va[j1]);) ++j1;
    for (; (a.ToVector() % va[j1].first.ToVector()) < 0;) ++j1;
    for (j2 = j1; va[j2].first == a;) ++j2;
  };
  for (; i < n; ++i) {
    AdjustJ();
    unsigned l1 = j1 - i, l2 = j2 - i;
    if ((l1 <= h) && (l2 >= h)) return va[i].second;
  }
  assert(false);
  return 0;
}

inline unsigned HalfSplittingLineAB(const std::vector<I2Point>& points,
                                    unsigned a) {
  assert((points.size() >= 2) && (a < points.size()));
  std::vector<I2Point> v;
  v.reserve(points.size() - 1);
  for (unsigned i = 0; i < points.size(); ++i) {
    if (i == a) continue;
    v.push_back((points[i] - points[a]).ToPoint());
  }
  unsigned b = HalfSplittingLine0B(v);
  return b + (b < a ? 0 : 1);
}
