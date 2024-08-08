#pragma once

#include "common/base.h"

namespace geometry {
namespace d2 {
template <class T>
class Point {
 public:
  using TSelf = Point<T>;

 public:
  T x, y;

 public:
  static consteval unsigned Dim() { return 2u; }

  constexpr Point() : x(), y() {}

  constexpr Point(const T& _x, const T& _y) : x(_x), y(_y) {}

  constexpr bool operator==(const TSelf& r) const {
    return (x == r.x) && (y == r.y);
  }

  constexpr bool operator!=(const TSelf& r) const {
    return (x != r.x) || (y != r.y);
  }

  constexpr bool operator<(const TSelf& r) const {
    return (x != r.x) ? (x < r.x) : (y < r.y);
  }

  constexpr T& operator[](unsigned index) {
    assert(index < 2);
    return (index == 0) ? x : y;
  }

  constexpr const T& operator[](unsigned index) const {
    assert(index < 2);
    return (index == 0) ? x : y;
  }

  constexpr TSelf operator-() const { return {-x, -y}; }
};
}  // namespace d2
}  // namespace geometry

using D2Point = geometry::d2::Point<double>;
using I2Point = geometry::d2::Point<int64_t>;
