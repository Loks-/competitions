#pragma once

#include "common/geometry/d3/point.h"
#include <istream>
#include <ostream>

template <class T>
inline std::ostream& operator<<(std::ostream& s,
                                const geometry::d3::Point<T>& p) {
  return s << p.x << " " << p.y << " " << p.z;
}

template <class T>
inline std::istream& operator>>(std::istream& s, geometry::d3::Point<T>& p) {
  return s >> p.x >> p.y >> p.z;
}
