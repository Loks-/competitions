#pragma once

#include "common/base.h"
#include "common/geometry/d3/point.h"

#include <cmath>

namespace geometry {
namespace d3 {
template <class TValue>
class Vector {
 public:
  using T = TValue;
  using TSelf = Vector<T>;

  T dx, dy, dz;

  consteval static unsigned Dim() { return 3u; }

  constexpr Vector() : dx(), dy(), dz() {}

  constexpr Vector(const T& _dx, const T& _dy, const T& _dz)
      : dx(_dx), dy(_dy), dz(_dz) {}

  constexpr Point<T> ToPoint() const { return Point<T>(dx, dy, dz); }

  constexpr bool Empty() const { return (dx == 0) && (dy == 0) && (dz == 0); }

  constexpr T LengthSquared() const { return dx * dx + dy * dy + dz * dz; }

  constexpr T Length() const { return sqrt(LengthSquared()); }

  constexpr bool operator==(const TSelf& r) const {
    return (dx == r.dx) && (dy == r.dy) && (dz == r.dz);
  }

  constexpr bool operator!=(const TSelf& r) const {
    return (dx != r.dx) || (dy != r.dy) || (dz != r.dz);
  }

  constexpr bool operator<(const TSelf& r) const {
    return (dx != r.dx)   ? (dx < r.dx)
           : (dy != r.dy) ? (dy < r.dy)
                          : (dz < r.dz);
  }

  constexpr T& operator[](unsigned index) {
    assert(index < 3);
    return (index == 0) ? dx : (index == 1) ? dy : dz;
  }

  constexpr const T& operator[](unsigned index) const {
    assert(index < 3);
    return (index == 0) ? dx : (index == 1) ? dy : dz;
  }

  constexpr TSelf operator-() const { return TSelf(-dx, -dy, -dz); }

  constexpr TSelf& operator*=(const T& r) {
    dx *= r;
    dy *= r;
    dz *= r;
    return *this;
  }

  constexpr TSelf& operator/=(const T& r) {
    dx /= r;
    dy /= r;
    dz /= r;
    return *this;
  }

  constexpr TSelf operator*(const T& r) const {
    return TSelf(dx * r, dy * r, dz * r);
  }

  constexpr TSelf operator/(const T& r) const {
    return TSelf(dx / r, dy / r, dz / r);
  }

  constexpr TSelf& operator+=(const TSelf& r) {
    dx += r.dx;
    dy += r.dy;
    dz += r.dz;
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& r) {
    dx -= r.dx;
    dy -= r.dy;
    dz -= r.dz;
    return *this;
  }

  constexpr TSelf operator+(const TSelf& r) const {
    return TSelf(dx + r.dx, dy + r.dy, dz + r.dz);
  }

  constexpr TSelf operator-(const TSelf& r) const {
    return TSelf(dx - r.dx, dy - r.dy, dz - r.dz);
  }

  constexpr T operator*(const TSelf& r) const {
    return dx * r.dx + dy * r.dy + dz * r.dz;
  }

  constexpr void Normalize() {
    assert(!Empty());
    operator/=(Length());
  }
};
}  // namespace d3
}  // namespace geometry

using D3Vector = geometry::d3::Vector<double>;
using I3Vector = geometry::d3::Vector<int64_t>;
