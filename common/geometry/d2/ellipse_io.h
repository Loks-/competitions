#pragma once

#include "common/geometry/d2/ellipse.h"
#include "common/geometry/d2/point_io.h"
#include <istream>
#include <ostream>

namespace geometry {
namespace d2 {
template <class T>
inline std::ostream& operator<<(std::ostream& s, const Ellipse<T>& p) {
  return s << p.f1 << " " << p.f2 << " " << p.a;
}

template <class T>
inline std::istream& operator>>(std::istream& s, Ellipse<T>& p) {
  return s >> p.f1 >> p.f2 >> p.a;
}
}  // namespace d2
}  // namespace geometry
