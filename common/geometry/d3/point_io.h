#pragma once

#include "common/geometry/d3/point.h"
#include <istream>
#include <ostream>

template <class T>
inline std::ostream& operator<<(std::ostream& s,
                                const geometry::d3::Point<T>& p) {
  s << p.x << " " << p.y << " " << p.z;
  return s;
}

template <class T>
inline std::istream& operator>>(std::istream& s, geometry::d3::Point<T>& p) {
  s >> p.x >> p.y >> p.z;
  return s;
}
