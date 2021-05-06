#pragma once

#include "common/geometry/d2/circle_int.h"
#include "common/geometry/d2/point_io.h"
#include <istream>
#include <ostream>

namespace geometry {
namespace d2 {
template <class T>
inline std::ostream& operator<<(std::ostream& s, const ICircle<T>& p) {
  return s << p.c << " " << p.r;
}

template <class T>
inline std::istream& operator>>(std::istream& s, ICircle<T>& p) {
  auto& r = s >> p.c >> p.r;
  p.UpdateR2();
  return r;
}
}  // namespace d2
}  // namespace geometry
