#pragma once

#include "common/base.h"
#include "common/geometry/d2/point.h"

#include <cmath>

namespace geometry {
namespace d2 {
template <class T>
class Vector {
 public:
  using TSelf = Vector<T>;

  T dx, dy;

  Vector() : dx(), dy() {}
  Vector(const T& _dx, const T& _dy) : dx(_dx), dy(_dy) {}
  explicit Vector(const Point<T>& p) : dx(p.x), dy(p.y) {}

  Point<T> ToPoint() const { return Point<T>(dx, dy); }
  bool Empty() const { return (dx == 0) && (dy == 0); }
  T LengthSquared() const { return dx * dx + dy * dy; }
  T Length() const { return sqrt(LengthSquared()); }

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

  TSelf operator*(const T& r) const { return TSelf(dx * r, dy * r); }
  TSelf operator/(const T& r) const { return TSelf(dx / r, dy / r); }
  TSelf operator-() const { return TSelf(-dx, -dy); }

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

  TSelf operator+(const TSelf& r) const { return TSelf(dx + r.dx, dy + r.dy); }
  TSelf operator-(const TSelf& r) const { return TSelf(dx - r.dx, dy - r.dy); }
  T operator*(const TSelf& r) const { return dx * r.dx + dy * r.dy; }
  T operator%(const TSelf& r) const { return dx * r.dy - dy * r.dx; }

  void Normalize() {
    assert(!Empty());
    operator/=(Length());
  }

  bool UpperHalfPlane() const { return (dy > 0) || ((dy == 0) && (dx >= 0)); }

  TSelf RotateHalfPi() const { return {-dy, dx}; }

  double RAngle() const {
    assert(!Empty());
    return atan2(dy, dx);
  }
};
}  // namespace d2
}  // namespace geometry

using D2Vector = geometry::d2::Vector<double>;
using I2Vector = geometry::d2::Vector<int64_t>;
