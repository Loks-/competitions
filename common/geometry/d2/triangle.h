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

 protected:
  static constexpr T zero = T(0);

 public:
  TPoint a, b, c;

 public:
  constexpr Triangle(const TPoint& _a, const TPoint& _b, const TPoint& _c)
      : a(_a), b(_b), c(_c) {}

  constexpr T SignedDoubleArea() const { return (b - a) % (c - a); }

  constexpr T SignedArea() const { return SignedDoubleArea() / 2; }

  constexpr T DoubleArea() const { return Abs(SignedDoubleArea()); }

  constexpr T Area() const { return DoubleArea() / 2; }

  constexpr bool Empty() const { return SignedDoubleArea() == zero; }

  constexpr bool Clockwise() const { return SignedDoubleArea() < zero; }

  constexpr bool Inside(const TPoint& p) const {
    const T d = SignedDoubleArea();
    const auto pa = p - a, pb = p - b, pc = p - c;
    if (d < zero) {
      return (pa % pc >= zero) && (pc % pb >= zero) && (pb % pa >= zero);
    } else if (zero < d) {
      return (pa % pb >= zero) && (pb % pc >= zero) && (pc % pa >= zero);
    } else {
      if (pb % pc != zero) return false;
      return Box(std::vector<TPoint>{a, b, c}).Inside(p);
    }
  }
};
}  // namespace d2
}  // namespace geometry
