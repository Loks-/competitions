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

  TPoint a, b, c;

  TrianglePA(const TPoint& _a, const TPoint& _b, const TPoint& _c)
      : a(_a), b(_b), c(_c) {
    assert(SignedDoubleArea() > 0);
  }

  T SignedDoubleArea() const { return (b - a) % (c - a); }
  T SignedArea() const { return SignedDoubleArea() / 2; }
  T DoubleArea() const { return SignedDoubleArea(); }
  T Area() const { return DoubleArea() / 2; }

  bool Empty() const { return false; }

  bool Inside(const TPoint& p) {
    T z(0);
    auto pa = p - a, pb = p - b, pc = p - c;
    return ((pa % pb) >= z) && ((pb % pc) >= z) && ((pc % pa) >= z);
  }
};
}  // namespace d2
}  // namespace geometry
