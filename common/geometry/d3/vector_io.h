#pragma once

#include "common/geometry/d3/vector.h"
#include <istream>
#include <ostream>

namespace geometry {
namespace d3 {
template <class T>
inline std::ostream& operator<<(std::ostream& s, const Vector<T>& v) {
  return s << v.dx << " " << v.dy << " " << v.dz;
}

template <class T>
inline std::istream& operator>>(std::istream& s, Vector<T>& v) {
  return s >> v.dx >> v.dy >> v.dz;
}
}  // namespace d3
}  // namespace geometry
