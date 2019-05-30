#pragma once

#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

template <class T>
inline geometry::d2::Point<T>& operator+=(geometry::d2::Point<T>& p,
                                          const geometry::d2::Vector<T>& v) {
  p.x += v.dx;
  p.y += v.dy;
  return p;
}

template <class T>
inline geometry::d2::Point<T>& operator-=(geometry::d2::Point<T>& p,
                                          const geometry::d2::Vector<T>& v) {
  p.x -= v.dx;
  p.y -= v.dy;
  return p;
}

template <class T>
inline geometry::d2::Point<T> operator+(const geometry::d2::Point<T>& p,
                                        const geometry::d2::Vector<T>& v) {
  return geometry::d2::Point<T>(p.x + v.dx, p.y + v.dy);
}

template <class T>
inline geometry::d2::Point<T> operator-(const geometry::d2::Point<T>& p,
                                        const geometry::d2::Vector<T>& v) {
  return geometry::d2::Point<T>(p.x - v.dx, p.y - v.dy);
}

template <class T>
inline geometry::d2::Vector<T> operator-(const geometry::d2::Point<T>& p1,
                                         const geometry::d2::Point<T>& p2) {
  return geometry::d2::Vector<T>(p1.x - p2.x, p1.y - p2.y);
}
