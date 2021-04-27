#pragma once

#include "common/geometry/d2/point.h"
#include <vector>

template <class T1, class T2>
inline std::vector<geometry::d2::Point<T1>>& ScaleX(
    std::vector<geometry::d2::Point<T1>>& points, const T2& s) {
  for (auto& p : points) p.x *= s;
  return points;
}

template <class T1, class T2>
inline std::vector<geometry::d2::Point<T1>>& ScaleY(
    std::vector<geometry::d2::Point<T1>>& points, const T2& s) {
  for (auto& p : points) p.y *= s;
  return points;
}

template <class T1, class T2>
inline std::vector<geometry::d2::Point<T1>>& ScaleXY(
    std::vector<geometry::d2::Point<T1>>& points, const T2& s) {
  for (auto& p : points) {
    p.x *= s;
    p.y *= s;
  }
  return points;
}
