#pragma once

#include "common/geometry/d2/ellipse.h"
#include "common/geometry/d2/point_io.h"
#include <istream>
#include <ostream>

template <class T>
inline std::ostream& operator<<(std::ostream& s,
                                const geometry::d2::Ellipse<T>& p) {
  return s << p.f1 << " " << p.f2 << " " << p.a;
}

template <class T>
inline std::istream& operator>>(std::istream& s, geometry::d2::Ellipse<T>& p) {
  return s >> p.f1 >> p.f2 >> p.a;
}
