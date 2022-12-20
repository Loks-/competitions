#pragma once

#include "common/geometry/ds/point.h"
#include "common/geometry/ds/vector.h"

template <unsigned dim, class T>
inline geometry::ds::Point<dim, T>& operator+=(
    geometry::ds::Point<dim, T>& p, const geometry::ds::Vector<dim, T>& v) {
  for (unsigned i = 0; i < dim; ++i) p[i] += v[i];
  return p;
}

template <unsigned dim, class T>
inline geometry::ds::Point<dim, T>& operator-=(
    geometry::ds::Point<dim, T>& p, const geometry::ds::Vector<dim, T>& v) {
  for (unsigned i = 0; i < dim; ++i) p[i] -= v[i];
  return p;
}

template <unsigned dim, class T>
inline geometry::ds::Point<dim, T> operator+(
    const geometry::ds::Point<dim, T>& p,
    const geometry::ds::Vector<dim, T>& v) {
  geometry::ds::Point<dim, T> p2;
  for (unsigned i = 0; i < dim; ++i) p2[i] = p[i] + v[i];
  return p2;
}

template <unsigned dim, class T>
inline geometry::ds::Point<dim, T> operator-(
    const geometry::ds::Point<dim, T>& p,
    const geometry::ds::Vector<dim, T>& v) {
  geometry::ds::Point<dim, T> p2;
  for (unsigned i = 0; i < dim; ++i) p2[i] = p[i] - v[i];
  return p2;
}

template <unsigned dim, class T>
inline geometry::ds::Vector<dim, T> operator-(
    const geometry::ds::Point<dim, T>& p1,
    const geometry::ds::Point<dim, T>& p2) {
  geometry::ds::Vector<dim, T> v;
  for (unsigned i = 0; i < dim; ++i) v[i] = p1[i] - p2[i];
  return v;
}
