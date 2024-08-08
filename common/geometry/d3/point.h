#pragma once

#include "common/base.h"

namespace geometry {
namespace d3 {
template <class T>
class Point {
 public:
  using TType = T;
  using TSelf = Point<T>;

 public:
  T x, y, z;

 public:
  static consteval unsigned Dim() { return 3u; }

  constexpr Point() : x(), y(), z() {}

  constexpr Point(const T& _x, const T& _y, const T& _z)
      : x(_x), y(_y), z(_z) {}

  constexpr bool operator==(const TSelf& r) const {
    return (x == r.x) && (y == r.y) && (z == r.z);
  }

  constexpr bool operator!=(const TSelf& r) const {
    return (x != r.x) || (y != r.y) || (z != r.z);
  }

  constexpr bool operator<(const TSelf& r) const {
    return (x != r.x) ? (x < r.x) : (y != r.y) ? (y < r.y) : (z < r.z);
  }

  constexpr T& operator[](unsigned index) {
    assert(index < 3);
    return (index == 0) ? x : (index == 1) ? y : z;
  }

  constexpr const T& operator[](unsigned index) const {
    assert(index < 3);
    return (index == 0) ? x : (index == 1) ? y : z;
  }

  constexpr TSelf operator-() const { return TSelf(-x, -y, -z); }
};
}  // namespace d3
}  // namespace geometry

using D3Point = geometry::d3::Point<double>;
using I3Point = geometry::d3::Point<int64_t>;
