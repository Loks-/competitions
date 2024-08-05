#pragma once

#include "common/base.h"
#include "common/geometry/d2/vector.h"

#include <cmath>

namespace geometry {
namespace d2 {
// Angle is represented as complex point dx + i*dy.
template <class T, class TTSelf>
class Angle {
 public:
  using TSelf = TTSelf;

  T dx, dy;

  constexpr Angle() : dx(1), dy(0) {}
  constexpr Angle(const T& _dx, const T& _dy) : dx(_dx), dy(_dy) {}
  constexpr Angle(const Vector<T>& v) : dx(v.dx), dy(v.dy) {}

  constexpr bool Valid() const { return (dx != 0) || (dy != 0); }

  constexpr Vector<T> ToVector() const { return Vector<T>(dx, dy); }

  constexpr double RAngle() const {
    assert(Valid());
    return atan2(dy, dx);
  }

  constexpr TSelf operator-() const { return TSelf(dx, -dy); }

  constexpr TSelf operator+(const TSelf& r) const {
    return TSelf(dx * r.dx - dy * r.dy, dx * r.dy + dy * r.dx);
  }

  constexpr TSelf operator-(const TSelf& r) const {
    return TSelf(dx * r.dx + dy * r.dy, -dx * r.dy + dy * r.dx);
  }

  constexpr TSelf ShiftPi() const { return TSelf(-dx, -dy); }

 protected:
  constexpr bool CompareI(const TSelf& r) const {
    return dy * r.dx < dx * r.dy;
  }

 public:
  // Compare angle with pi keeping angle in the range [0, 2pi).
  constexpr bool CompareVSPi() const {
    return (dy > 0) || ((dy == 0) && (dx > 0));
  }

  // Compare angle with 0 keeping angle in the range [-pi, pi).
  constexpr bool CompareVS0() const {
    return (dy < 0) || ((dy == 0) && (dx < 0));
  }

  // Compare angles keeping them in the range [-pi, pi)
  constexpr bool ComparePiPi(const TSelf& r) const {
    return (CompareVS0() != r.CompareVS0()) ? CompareVS0() : CompareI(r);
  }

  // Compare angles keeping them in the range [0, 2pi)
  constexpr bool Compare02Pi(const TSelf& r) const {
    return (CompareVSPi() != r.CompareVSPi()) ? CompareVSPi() : CompareI(r);
  }

  constexpr bool operator==(const TSelf& r) const {
    return (dx == r.dx) && (dy == r.dy);
  }

  constexpr bool operator!=(const TSelf& r) const {
    return (dx != r.dx) || (dy != r.dy);
  }

  constexpr bool operator<(const TSelf& r) const { return ComparePiPi(r); }
  constexpr bool operator>(const TSelf& r) const { return r < *this; }
  constexpr bool operator<=(const TSelf& r) const { return !(r < *this); }
  constexpr bool operator>=(const TSelf& r) const { return !(*this < r); }
};
}  // namespace d2
}  // namespace geometry
