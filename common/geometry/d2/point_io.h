#pragma once

#include "common/geometry/d2/point.h"
#include <istream>
#include <ostream>

template <class T>
inline std::ostream& operator<<(std::ostream& s,
                                const geometry::d2::Point<T>& p) {
  s << p.x << " " << p.y;
  return s;
}

template <class T>
inline std::istream& operator>>(std::istream& s, geometry::d2::Point<T>& p) {
  s >> p.x >> p.y;
  return s;
}
