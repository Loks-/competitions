#pragma once

#include "common/base.h"

namespace geometry {
namespace d2 {
template <class TValue>
class Point {
 public:
  using T = TValue;
  using TSelf = Point<T>;
  static const unsigned dim = 2;

  T x, y;

  Point() : x(), y() {}
  Point(const T& _x, const T& _y) : x(_x), y(_y) {}

  bool operator==(const TSelf& r) const { return (x == r.x) && (y == r.y); }
  bool operator!=(const TSelf& r) const { return (x != r.x) || (y != r.y); }
  bool operator<(const TSelf& r) const {
    return (x != r.x) ? (x < r.x) : (y < r.y);
  }

  T& operator[](unsigned index) {
    assert(index < 2);
    return (index == 0) ? x : y;
  }

  const T& operator[](unsigned index) const {
    assert(index < 2);
    return (index == 0) ? x : y;
  }

  TSelf operator-() const { return {-x, -y}; }
};
}  // namespace d2
}  // namespace geometry

using D2Point = geometry::d2::Point<double>;
using I2Point = geometry::d2::Point<int64_t>;
