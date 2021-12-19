#pragma once

#include "common/base.h"

#include <cmath>

namespace geometry {
namespace d3 {
template <class T>
class Vector {
 public:
  using TSelf = Vector<T>;

  T dx, dy, dz;

  Vector() : dx(), dy(), dz() {}
  Vector(const T& _dx, const T& _dy, const T& _dz)
      : dx(_dx), dy(_dy), dz(_dz) {}

  Point<T> ToPoint() const { return Point<T>(dx, dy, dz); }
  bool Empty() const { return (dx == 0) && (dy == 0) && (dz == 0); }
  T LengthSquared() const { return dx * dx + dy * dy + dz * dz; }
  T Length() const { return sqrt(LengthSquared()); }

  bool operator==(const TSelf& v) const {
    return (dx == v.dx) && (dy == v.dy) && (dz == v.dz);
  }

  bool operator<(const TSelf& v) const {
    return (dx != v.dx)   ? (dx < v.dx)
           : (dy != v.dy) ? (dy < v.dy)
                          : (dz < v.dz);
  }

  T& operator[](unsigned index) {
    assert(index < 3);
    return (index == 0) ? dx : (index == 1) ? dy : dz;
  }

  const T& operator[](unsigned index) const {
    assert(index < 3);
    return (index == 0) ? dx : (index == 1) ? dy : dz;
  }

  TSelf& operator+=(const TSelf& r) {
    dx += r.dx;
    dy += r.dy;
    dz += r.dz;
    return *this;
  }

  TSelf& operator-=(const TSelf& r) {
    dx -= r.dx;
    dy -= r.dy;
    dz -= r.dz;
    return *this;
  }

  TSelf& operator*=(const T& r) {
    dx *= r;
    dy *= r;
    dz *= r;
    return *this;
  }

  TSelf& operator/=(const T& r) {
    dx /= r;
    dy /= r;
    dz /= r;
    return *this;
  }

  TSelf operator+(const TSelf& r) const {
    return TSelf(dx + r.dx, dy + r.dy, dz + r.dz);
  }

  TSelf operator-(const TSelf& r) const {
    return TSelf(dx - r.dx, dy - r.dy, dz - r.dz);
  }

  TSelf operator*(const T& r) const { return TSelf(dx * r, dy * r, dz * r); }
  TSelf operator/(const T& r) const { return TSelf(dx / r, dy / r, dz / r); }
  TSelf operator-() const { return TSelf(-dx, -dy, -dz); }
  T operator*(const TSelf& r) const {
    return dx * r.dx + dy * r.dy + dz * r.dz;
  }

  void Normalize() {
    assert(!Empty());
    operator/=(Length());
  }
};
}  // namespace d3
}  // namespace geometry

using D3Vector = geometry::d3::Vector<double>;
using I3Vector = geometry::d3::Vector<int64_t>;
