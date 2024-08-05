#pragma once

#include "common/base.h"
#include "common/geometry/ds/point.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace geometry {
namespace ds {
template <unsigned dim, class TValue>
class Vector {
 public:
  using T = TValue;
  using TSelf = Vector<dim, T>;

  std::array<T, dim> dpc;

  consteval static unsigned Dim() { return dim; }

  constexpr Vector() { std::fill(dpc.begin(), dpc.end(), T()); }

  constexpr explicit Vector(const T& value) {
    std::fill(dpc.begin(), dpc.end(), value);
  }

  constexpr explicit Vector(const std::array<T, dim>& v) : dpc(v) {}

  constexpr explicit Vector(const std::vector<T>& v) {
    assert(v.size() == dim);
    for (unsigned i = 0; i < dim; ++i) dpc[i] = v[i];
  }

  constexpr Point<dim, T> ToPoint() const { return Point<dim, T>(dpc); }

  constexpr bool Empty() const {
    for (auto x : dpc) {
      if (x != 0) return false;
    }
    return true;
  }

  constexpr T LengthSquared() const {
    T s = 0;
    for (auto x : dpc) s += x * x;
    return s;
  }

  constexpr T Length() const { return sqrt(LengthSquared()); }

  constexpr bool operator==(const TSelf& r) const { return dpc == r.dpc; }

  constexpr bool operator!=(const TSelf& r) const { return dpc != r.dpc; }

  constexpr bool operator<(const TSelf& r) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (dpc[i] != r[i]) return dpc[i] < r[i];
    }
    return false;
  }

  constexpr T& operator[](unsigned index) { return dpc[index]; }

  constexpr const T& operator[](unsigned index) const { return dpc[index]; }

  constexpr TSelf operator-() const {
    TSelf p;
    for (unsigned i = 0; i < dim; ++i) p[i] = -dpc[i];
    return p;
  }

  constexpr TSelf& operator*=(const T& r) {
    for (auto& x : dpc) x *= r;
    return *this;
  }

  constexpr TSelf& operator/=(const T& r) {
    for (auto& x : dpc) x /= r;
    return *this;
  }

  constexpr TSelf operator*(const T& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  constexpr TSelf operator/(const T& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  constexpr TSelf& operator+=(const TSelf& r) {
    for (unsigned i = 0; i < dim; ++i) dpc[i] += r[i];
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& r) {
    for (unsigned i = 0; i < dim; ++i) dpc[i] -= r[i];
    return *this;
  }

  constexpr TSelf operator+(const TSelf& r) const {
    TSelf t(*this);
    t += r;
    return t;
  }

  constexpr TSelf operator-(const TSelf& r) const {
    TSelf t(*this);
    t -= r;
    return t;
  }

  constexpr T operator*(const TSelf& r) const {
    T s = 0;
    for (unsigned i = 0; i < dim; ++i) s += dpc[i] * r[i];
    return s;
  }

  constexpr void Normalize() {
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
