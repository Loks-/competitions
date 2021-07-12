#pragma once

#include "common/base.h"

namespace geometry {
namespace d2 {
template <class TValue>
class Point {
 public:
  using T = TValue;
  using TSelf = Point<T>;

  T x, y;

  Point() : x(), y() {}
  Point(const T& _x, const T& _y) : x(_x), y(_y) {}

  bool operator==(const TSelf& r) const { return (x == r.x) && (y == r.y); }
  bool operator!=(const TSelf& r) const { return (x != r.x) || (y != r.y); }
};
}  // namespace d2
}  // namespace geometry

using D2Point = geometry::d2::Point<double>;
using I2Point = geometry::d2::Point<int64_t>;
