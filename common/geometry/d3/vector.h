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

  bool Empty() const { return (dx == 0) && (dy == 0) && (dz == 0); }
  T LengthSquared() const { return dx * dx + dy * dy + dz * dz; }
  T Length() const { return sqrt(LengthSquared()); }

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
