#pragma once

#include "common/geometry/d3/vector.h"
#include <istream>
#include <ostream>

template <class T>
inline std::ostream& operator<<(std::ostream& s,
                                const geometry::d3::Vector<T>& v) {
  return s << v.dx << " " << v.dy << " " << v.dz;
}

template <class T>
inline std::istream& operator>>(std::istream& s, geometry::d3::Vector<T>& v) {
  return s >> v.dx >> v.dy >> v.dz;
}
