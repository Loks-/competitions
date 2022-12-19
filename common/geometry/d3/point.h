#pragma once

#include "common/base.h"

namespace geometry {
namespace d3 {
template <class TValue>
class Point {
 public:
  using T = TValue;
  using TSelf = Point<T>;
  static const unsigned dim = 3;

  T x, y, z;

  Point() : x(), y(), z() {}
  Point(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}

  bool operator==(const TSelf& r) const {
    return (x == r.x) && (y == r.y) && (z == r.z);
  }

  bool operator!=(const TSelf& r) const {
    return (x != r.x) || (y != r.y) || (z != r.z);
  }

  bool operator<(const TSelf& r) const {
    return (x != r.x) ? (x < r.x) : (y != r.y) ? (y < r.y) : (z < r.z);
  }

  T& operator[](unsigned index) {
    assert(index < 3);
    return (index == 0) ? x : (index == 1) ? y : z;
  }

  const T& operator[](unsigned index) const {
    assert(index < 3);
    return (index == 0) ? x : (index == 1) ? y : z;
  }

  TSelf operator-() const { return TSelf(-x, -y, -z); }
};
}  // namespace d3
}  // namespace geometry

using D3Point = geometry::d3::Point<double>;
using I3Point = geometry::d3::Point<int64_t>;
