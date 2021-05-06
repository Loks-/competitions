#pragma once

#include "common/geometry/d2/point.h"

template <class T1, class T2>
inline geometry::d2::Point<T1> ScaleX(const geometry::d2::Point<T1>& p,
                                      const T2& s) {
  return {p.x * s, p.y};
}

template <class T1, class T2>
inline geometry::d2::Point<T1> ScaleY(const geometry::d2::Point<T1>& p,
                                      const T2& s) {
  return {p.x, p.y * s};
}

template <class T1, class T2>
inline geometry::d2::Point<T1> ScaleXY(const geometry::d2::Point<T1>& p,
                                       const T2& s) {
  return {p.x * s, p.y * s};
}
