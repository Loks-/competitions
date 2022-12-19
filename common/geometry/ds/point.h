#pragma once

#include "common/base.h"

#include <algorithm>
#include <array>
#include <vector>

namespace geometry {
namespace ds {
template <unsigned dim_, class TValue>
class Point {
 public:
  using T = TValue;
  using TSelf = Point<dim_, T>;
  static const unsigned dim = dim_;

  std::array<dim, T> pc;

  Point() { std::fill(pc.begin(), pc.end(), T()); }
  Point(const std::array<dim, T>& v) : pc(v) {}

  Point(const std::vector<T>& v) {
    assert(v.size() == dim);
    for (unsigned i = 0; i < dim; ++i) pc[i] = v[i];
  }

  bool operator==(const TSelf& r) const { return pc == r.pc; }
  bool operator!=(const TSelf& r) const { return pc != r.pc; }

  bool operator<(const TSelf& r) const {
    for (unsigned i = 0; i < dim; ++i) {
      if (pc[i] != r[i]) return pc[i] < r[i];
    }
    return false;
  }

  T& operator[](unsigned index) { return pc[index]; }
  const T& operator[](unsigned index) const { return pc[index]; }

  TSelf operator-() const {
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
