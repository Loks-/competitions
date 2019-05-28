#pragma once

namespace geometry {
namespace d3 {
template <class T>
class Point {
 public:
  T x, y, z;

  Point() : x(), y(), z() {}
  Point(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
};
}  // namespace d3
}  // namespace geometry

using D3Point = geometry::d3::Point<double>;
