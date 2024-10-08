#pragma once

#include "common/geometry/d2/point.h"

namespace geometry {
namespace kdtree {
namespace base {
template <class T>
constexpr bool Under(const geometry::d2::Point<T>& l,
                     const geometry::d2::Point<T>& r) {
  return (l.x <= r.x) && (l.y <= r.y);
}

template <class T>
constexpr bool StrictUnder(const geometry::d2::Point<T>& l,
                           const geometry::d2::Point<T>& r) {
  return (l.x < r.x) && (l.y < r.y);
}
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
