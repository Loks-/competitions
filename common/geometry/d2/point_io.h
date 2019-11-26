#pragma once

#include "common/geometry/d2/point.h"
#include <istream>
#include <ostream>

namespace geometry {
namespace d2 {
template <class T>
inline std::ostream& operator<<(std::ostream& s, const Point<T>& p) {
  return s << p.x << " " << p.y;
}

template <class T>
inline std::istream& operator>>(std::istream& s, Point<T>& p) {
  return s >> p.x >> p.y;
}
}  // namespace d2
}  // namespace geometry
