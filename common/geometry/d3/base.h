#pragma once

#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

template <class T>
inline geometry::d3::Point<T>& operator+=(geometry::d3::Point<T>& p,
                                          const geometry::d3::Vector<T>& v) {
  p.x += v.dx;
  p.y += v.dy;
  p.z += v.dz;
  return p;
}

template <class T>
inline geometry::d3::Point<T>& operator-=(geometry::d3::Point<T>& p,
                                          const geometry::d3::Vector<T>& v) {
  p.x -= v.dx;
  p.y -= v.dy;
  p.z -= v.dz;
  return p;
}

template <class T>
inline geometry::d3::Point<T> operator+(const geometry::d3::Point<T>& p,
                                        const geometry::d3::Vector<T>& v) {
  return geometry::d3::Point<T>(p.x + v.dx, p.y + v.dy, p.z + v.dz);
}

template <class T>
inline geometry::d3::Point<T> operator-(const geometry::d3::Point<T>& p,
                                        const geometry::d3::Vector<T>& v) {
  return geometry::d3::Point<T>(p.x - v.dx, p.y - v.dy, p.z - v.dz);
}

template <class T>
inline geometry::d3::Vector<T> operator-(const geometry::d3::Point<T>& p1,
                                         const geometry::d3::Point<T>& p2) {
  return geometry::d3::Vector<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}
