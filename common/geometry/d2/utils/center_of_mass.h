#pragma once

#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

#include <vector>

template <class T>
inline geometry::d2::Vector<T> CenterOfMass(
    const std::vector<geometry::d2::Vector<T>>& vectors) {
  if (vectors.empty()) return {};
  geometry::d2::Vector<T> s;
  for (auto& v : vectors) s += v;
  return s / vectors.size();
}

template <class T>
inline geometry::d2::Point<T> CenterOfMass(
    const std::vector<geometry::d2::Point<T>>& points) {
  unsigned n = points.size();
  if (n == 0) return {};
  T sx(0), sy(0);
  for (auto& p : points) {
    sx += p.x;
    sy += p.y;
  }
  return geometry::d2::Point<T>(sx / n, sy / n);
}
