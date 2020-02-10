#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/utils/box.h"
#include "common/numeric/utils/abs.h"
#include <vector>

namespace geometry {
namespace d2 {
template <class T>
class Triangle {
 public:
  using TPoint = Point<T>;

  TPoint a, b, c;

  Triangle(const TPoint& _a, const TPoint& _b, const TPoint& _c)
      : a(_a), b(_b), c(_c) {}

  T SignedDoubleArea() const { return (b - a) % (c - a); }
  T SignedArea() const { return SignedDoubleArea() / 2; }
  T DoubleArea() const { return Abs(SignedDoubleArea); }
  T Area() const { return DoubleArea() / 2; }

  bool Empty() const { return SignedDoubleArea() == T(0); }
  bool Clockwise() const { return SignedDoubleArea() < T(0); }

  bool Inside(const TPoint& p) {
    T z(0), d = SignedDoubleArea();
    if (d < z) {
      return ((p - a) % (p - c) >= z) && ((p - c) % (p - b) >= z) &&
             ((p - b) % (p - a) >= z);
    } else if (z < d) {
      return ((p - a) % (p - b) >= z) && ((p - b) % (p - c) >= z) &&
             ((p - c) % (p - a) >= z);
    } else {
      if ((p - b) % (p - c) != z) return false;
      return Box(std::vector<TPoint>{a, b, c}).Inside(p);
    }
  }
};
}  // namespace d2
}  // namespace geometry
