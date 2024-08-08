#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"

#include <vector>

namespace geometry {
namespace d2 {
// Vertexes are saved in counterclockwise order
template <class T>
class TrianglePA {
 public:
  using TPoint = Point<T>;

 protected:
  TPoint a, b, c;

 public:
  constexpr TrianglePA(const TPoint& _a, const TPoint& _b, const TPoint& _c)
      : a(_a), b(_b), c(_c) {
    assert(SignedDoubleArea() > 0);
  }

  constexpr T SignedDoubleArea() const { return (b - a) % (c - a); }

  constexpr T SignedArea() const { return SignedDoubleArea() / 2; }

  constexpr T DoubleArea() const { return SignedDoubleArea(); }

  constexpr T Area() const { return DoubleArea() / 2; }

  constexpr bool Empty() const { return false; }

  constexpr bool Inside(const TPoint& p) const {
    constexpr T z(0);
    const auto pa = p - a, pb = p - b, pc = p - c;
    return ((pa % pb) >= z) && ((pb % pc) >= z) && ((pc % pa) >= z);
  }
};
}  // namespace d2
}  // namespace geometry
