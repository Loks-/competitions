#pragma once

#include "common/geometry/ds/point.h"
#include "common/geometry/ds/vector.h"

namespace geometry {
namespace ds {
template <unsigned dim, class T>
inline Point<dim, T>& operator+=(Point<dim, T>& p, const Vector<dim, T>& v) {
  for (unsigned i = 0; i < dim; ++i) p[i] += v[i];
  return p;
}

template <unsigned dim, class T>
inline Point<dim, T>& operator-=(Point<dim, T>& p, const Vector<dim, T>& v) {
  for (unsigned i = 0; i < dim; ++i) p[i] -= v[i];
  return p;
}

template <unsigned dim, class T>
inline Point<dim, T> operator+(const Point<dim, T>& p,
                               const Vector<dim, T>& v) {
  Point<dim, T> p2;
  for (unsigned i = 0; i < dim; ++i) p2[i] = p[i] + v[i];
  return p2;
}

template <unsigned dim, class T>
inline Point<dim, T> operator-(const Point<dim, T>& p,
                               const Vector<dim, T>& v) {
  Point<dim, T> p2;
  for (unsigned i = 0; i < dim; ++i) p2[i] = p[i] - v[i];
  return p2;
}

template <unsigned dim, class T>
inline Vector<dim, T> operator-(const Point<dim, T>& p1,
                                const Point<dim, T>& p2) {
  Vector<dim, T> v;
  for (unsigned i = 0; i < dim; ++i) v[i] = p1[i] - p2[i];
  return v;
}
}  // namespace ds
}  // namespace geometry
