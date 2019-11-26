#pragma once

#include "common/geometry/d3/point.h"
#include <istream>
#include <ostream>

namespace geometry {
namespace d3 {
template <class T>
inline std::ostream& operator<<(std::ostream& s, const Point<T>& p) {
  return s << p.x << " " << p.y << " " << p.z;
}

template <class T>
inline std::istream& operator>>(std::istream& s, Point<T>& p) {
  return s >> p.x >> p.y >> p.z;
}
}  // namespace d3
}  // namespace geometry
