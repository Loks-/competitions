#pragma once

#include "common/geometry/d2/vector.h"

template <class T>
inline bool CompareVectorAngle(const geometry::d2::Vector<T>& v1,
                               const geometry::d2::Vector<T>& v2) {
  if (v1.UpperHalfPlane() != v2.UpperHalfPlane()) return v1.UpperHalfPlane();
  return (v2 % v1) < 0;
}
