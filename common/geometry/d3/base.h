#pragma once

#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

namespace geometry {
namespace d3 {
template <class T>
inline Point<T>& operator+=(Point<T>& p, const Vector<T>& v) {
  p.x += v.dx;
  p.y += v.dy;
  p.z += v.dz;
  return p;
}

template <class T>
inline Point<T>& operator-=(Point<T>& p, const Vector<T>& v) {
  p.x -= v.dx;
  p.y -= v.dy;
  p.z -= v.dz;
  return p;
}

template <class T>
inline Point<T> operator+(const Point<T>& p, const Vector<T>& v) {
  return Point<T>(p.x + v.dx, p.y + v.dy, p.z + v.dz);
}

template <class T>
inline Point<T> operator-(const Point<T>& p, const Vector<T>& v) {
  return Point<T>(p.x - v.dx, p.y - v.dy, p.z - v.dz);
}

template <class T>
inline Vector<T> operator-(const Point<T>& p1, const Point<T>& p2) {
  return Vector<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}
}  // namespace d3
}  // namespace geometry
