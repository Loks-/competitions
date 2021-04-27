#pragma once

#include "common/geometry/d2/vector.h"
#include <istream>
#include <ostream>

namespace geometry {
namespace d2 {
template <class T>
inline std::ostream& operator<<(std::ostream& s, const Vector<T>& v) {
  return s << v.dx << " " << v.dy;
}

template <class T>
inline std::istream& operator>>(std::istream& s, Vector<T>& v) {
  return s >> v.dx >> v.dy;
}
}  // namespace d2
}  // namespace geometry
