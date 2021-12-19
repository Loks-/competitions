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

  bool operator==(const TSelf& p) const {
    return (x == p.x) && (y == p.y) && (z == p.z);
  }

  bool operator!=(const TSelf& p) const {
    return (x != p.x) || (y != p.y) || (z != p.z);
  }

  bool operator<(const TSelf& v) const {
    return (x != v.x) ? (x < v.x) : (y != v.y) ? (y < v.y) : (z < v.z);
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
