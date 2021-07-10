#pragma once

#include "common/geometry/d2/angle_int.h"

namespace geometry {
namespace d2 {
// Check if testedangle between a1 and a2 if we assume clockwise direction.
template <class T>
inline bool Inside(const IAngle<T>& a1, 
                   const IAngle<T>& a2,
                   const IAngle<T>& atest) {
  auto b2 = a2 - a1, bt = atest - a1;
  return bt.Compare02Pi(b2);
}
}  // namespace d2
}  // namespace geometry
