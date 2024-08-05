#pragma once

#include "common/base.h"
#include "common/geometry/d2/point.h"

#include <cmath>

namespace geometry {
namespace d2 {
template <class TValue>
class Vector {
 public:
  using T = TValue;
  using TSelf = Vector<T>;

  T dx, dy;

  consteval static unsigned Dim() { return 2u; }

  constexpr Vector() : dx(), dy() {}
  constexpr Vector(const T& _dx, const T& _dy) : dx(_dx), dy(_dy) {}
  constexpr explicit Vector(const Point<T>& p) : dx(p.x), dy(p.y) {}

  constexpr Point<T> ToPoint() const { return Point<T>(dx, dy); }
  constexpr bool Empty() const { return (dx == 0) && (dy == 0); }
  constexpr T LengthSquared() const { return dx * dx + dy * dy; }
  constexpr T Length() const { return sqrt(LengthSquared()); }

  constexpr bool operator==(const TSelf& r) const {
    return (dx == r.dx) && (dy == r.dy);
  }

  constexpr bool operator!=(const TSelf& r) const {
    return (dx != r.dx) || (dy != r.dy);
  }

  constexpr bool operator<(const TSelf& r) const {
    return (dx != r.dx) ? (dx < r.dx) : (dy < r.dy);
  }

  T& operator[](unsigned index) {
    assert(index < 2);
    return (index == 0) ? dx : dy;
  }

  constexpr const T& operator[](unsigned index) const {
    assert(index < 2);
    return (index == 0) ? dx : dy;
  }

  constexpr TSelf operator-() const { return TSelf(-dx, -dy); }

  TSelf& operator*=(const T& r) {
    dx *= r;
    dy *= r;
    return *this;
  }

  TSelf& operator/=(const T& r) {
    dx /= r;
    dy /= r;
    return *this;
  }

  constexpr TSelf operator*(const T& r) const { return TSelf(dx * r, dy * r); }
  constexpr TSelf operator/(const T& r) const { return TSelf(dx / r, dy / r); }

  TSelf& operator+=(const TSelf& r) {
    dx += r.dx;
    dy += r.dy;
    return *this;
  }

  TSelf& operator-=(const TSelf& r) {
    dx -= r.dx;
    dy -= r.dy;
    return *this;
  }

  constexpr TSelf operator+(const TSelf& r) const {
    return TSelf(dx + r.dx, dy + r.dy);
  }

  constexpr TSelf operator-(const TSelf& r) const {
    return TSelf(dx - r.dx, dy - r.dy);
  }

  constexpr T operator*(const TSelf& r) const { return dx * r.dx + dy * r.dy; }
  constexpr T operator%(const TSelf& r) const { return dx * r.dy - dy * r.dx; }

  void Normalize() {
    assert(!Empty());
    operator/=(Length());
  }

  constexpr bool UpperHalfPlane() const {
    return (dy > 0) || ((dy == 0) && (dx >= 0));
  }

  constexpr TSelf RotateHalfPi() const { return {-dy, dx}; }

  constexpr double RAngle() const {
    assert(!Empty());
    return atan2(dy, dx);
  }
};
}  // namespace d2
}  // namespace geometry

using D2Vector = geometry::d2::Vector<double>;
using I2Vector = geometry::d2::Vector<int64_t>;
