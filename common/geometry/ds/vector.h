#pragma once

#include "common/base.h"
#include "common/geometry/ds/point.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace geometry {
namespace ds {
template <unsigned dim_, class TValue>
class Vector {
 public:
  using T = TValue;
  using TSelf = Vector<dim_, T>;
  static const unsigned dim = dim_;

  std::array<dim, T> dpc;

  Vector() { std::fill(dpc.begin(), dpc.end(), T()); }
  Vector(const std::array<dim, T>& v) : dpc(v) {}

  Vector(const std::vector<T>& v) {
    assert(v.size() == dim);
    for (unsigned i = 0; i < dim; ++i) dpc[i] = v[i];
  }

  Point<dim, T> ToPoint() const { return Point<dim, T>(dpc); }

  bool Empty() const {
    for (auto x : dpc) {
      if (x != 0) return false;
    }
    return true;
  }

  T LengthSquared() const {
    T s = 0;
    for (auto x : dpc) s += x * x;
    return s;
  }

  T Length() const { return sqrt(LengthSquared()); }

  bool operator==(const TSelf& r) const { return dpc == r.dpc; }
  bool operator!=(const TSelf& r) const { return dpc != r.dpc; }

  bool operator<(const TSelf& r) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (pc[i] != r[i]) return pc[i] < r[i];
    }
    return false;
  }

  T& operator[](unsigned index) { return dpc[index]; }
  const T& operator[](unsigned index) const { return dpc[index]; }

  TSelf operator-() const {
    TSelf p;
    for (unsigned i = 0; i < dim; ++i) p[i] = -pc[i];
    return p;
  }

  TSelf& operator*=(const T& r) {
    for (auto& x : dpc) x *= r;
    return *this;
  }

  TSelf& operator/=(const T& r) {
    for (auto& x : dpc) x /= r;
    return *this;
  }

  TSelf operator*(const T& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  TSelf operator/(const T& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  TSelf& operator+=(const TSelf& r) {
    for (unsigned i = 0; i < dim; ++i) dpc[i] += r[i];
    return *this;
  }

  TSelf& operator-=(const TSelf& r) {
    for (unsigned i = 0; i < dim; ++i) dpc[i] -= r[i];
    return *this;
  }

  TSelf operator+(const TSelf& r) const {
    TSelf t(*this);
    t += r;
    return t;
  }

  TSelf operator-(const TSelf& r) const {
    TSelf t(*this);
    t -= r;
    return t;
  }

  T operator*(const TSelf& r) const {
    T s = 0;
    for (unsigned i = 0; i < dim; ++i) s += dpc[i] * r[i];
    return s;
  }

  void Normalize() {
    assert(!Empty());
    operator/=(Length());
  }
};
}  // namespace ds
}  // namespace geometry

template <unsigned dim>
using DSVector = geometry::ds::Vector<dim, double>;

template <unsigned dim>
using ISVector = geometry::ds::Vector<dim, int64_t>;
