#pragma once

#include "common/base.h"

#include <algorithm>
#include <array>
#include <vector>

namespace geometry {
namespace ds {
template <unsigned dim, class TValue>
class Point {
 public:
  using T = TValue;
  using TSelf = Point<dim, T>;

  std::array<T, dim> pc;

  consteval static unsigned Dim() { return dim; }

  constexpr Point() { std::fill(pc.begin(), pc.end(), T()); }

  constexpr explicit Point(const T& value) {
    std::fill(pc.begin(), pc.end(), value);
  }

  constexpr explicit Point(const std::array<T, dim>& v) : pc(v) {}

  constexpr explicit Point(const std::vector<T>& v) {
    assert(v.size() == dim);
    for (unsigned i = 0; i < dim; ++i) pc[i] = v[i];
  }

  constexpr bool operator==(const TSelf& r) const { return pc == r.pc; }
  constexpr bool operator!=(const TSelf& r) const { return pc != r.pc; }

  constexpr bool operator<(const TSelf& r) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (pc[i] != r[i]) return pc[i] < r[i];
    }
    return false;
  }

  constexpr T& operator[](unsigned index) { return pc[index]; }

  constexpr const T& operator[](unsigned index) const { return pc[index]; }

  constexpr TSelf operator-() const {
    TSelf p;
    for (unsigned i = 0; i < dim; ++i) p[i] = -pc[i];
    return p;
  }
};
}  // namespace ds
}  // namespace geometry

template <unsigned dim>
using DSPoint = geometry::ds::Point<dim, double>;

template <unsigned dim>
using ISPoint = geometry::ds::Point<dim, int64_t>;
