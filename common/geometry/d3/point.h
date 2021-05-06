#pragma once

#include "common/base.h"

namespace geometry {
namespace d3 {
template <class TValue>
class Point {
 public:
  using T = TValue;
  using TSelf = Point<T>;

  T x, y, z;

  Point() : x(), y(), z() {}
  Point(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
};
}  // namespace d3
}  // namespace geometry

using D3Point = geometry::d3::Point<double>;
using I3Point = geometry::d3::Point<int64_t>;
