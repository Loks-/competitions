#pragma once

#include "common/base.h"
#include "common/geometry/d2/angle_int.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_xy.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"
#include "common/stl/pair.h"

#include <algorithm>
#include <utility>
#include <vector>

// Find a line that split set of points into two halves.
template <class T, class TAngle = geometry::d2::IAngle<T>>
inline std::pair<unsigned, unsigned> HalfSplittingLineAB(
    const std::vector<geometry::d2::Point<T>>& points) {
  assert(points.size() >= 2);
  unsigned a = 0;
  for (unsigned i = 1; i < points.size(); ++i) {
    if (CompareXY(points[i], points[a])) a = i;
  }
  std::vector<std::pair<TAngle, unsigned>> va;
  va.reserve(points.size() - 1);
  for (unsigned i = 0; i < points.size(); ++i) {
    if (i == a) continue;
    va.push_back({TAngle(points[i] - points[a]), i});
  }
  unsigned m = va.size() / 2;
  std::nth_element(va.begin(), va.begin() + m, va.end());
  return {a, va[m].second};
}

template <class T, class TAngle = geometry::d2::IAngle<T>>
inline unsigned HalfSplittingLine0B(
    const std::vector<geometry::d2::Point<T>>& points,
    geometry::d2::Point<T> p0 = geometry::d2::Point<T>()) {
  thread_local std::vector<std::pair<TAngle, unsigned>> va;
  va.clear();
  for (unsigned i = 0; i < points.size(); ++i) {
    if (points[i] == p0) continue;
    va.push_back({TAngle(points[i] - p0), i});
  }
  unsigned n = va.size(), h = n / 2 + 1;
  assert(n >= 1);
  std::sort(va.begin(), va.end());
  if (va.back().first == va[0].first) return va[0].second;
  unsigned i = 0, j1 = 0, j2 = 0;
  auto AdjustJ = [&]() {
    auto a = va[i].first.ShiftPi();
    for (; (j1 < n) && (va[i] == va[j1]);) ++j1;
    for (; (a.ToVector() % va[j1 % n].first.ToVector()) < 0;) ++j1;
    for (j2 = j1; va[j2 % n].first == a;) ++j2;
  };
  for (; i < n; ++i) {
    AdjustJ();
    unsigned l1 = j1 - i, l2 = j2 - i;
    if ((l1 <= h) && (l2 >= h)) return va[i].second;
  }
  assert(false);
  return 0;
}

template <class T, class TAngle = geometry::d2::IAngle<T>>
inline unsigned HalfSplittingLineAB(
    const std::vector<geometry::d2::Point<T>>& points, unsigned a) {
  return HalfSplittingLine0B(points, points[a]);
}
