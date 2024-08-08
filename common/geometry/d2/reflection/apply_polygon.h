#pragma once

#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/reflection.h"

#include <vector>

namespace geometry {
namespace d2 {
template <class T>
constexpr Polygon<T> Apply(const Reflection<T>& r, const Polygon<T>& plgn) {
  std::vector<Point<T>> vp;
  for (auto& p : plgn.Vertices()) vp.push_back(r(p));
  return Polygon<T>(vp);
}
}  // namespace d2
}  // namespace geometry
