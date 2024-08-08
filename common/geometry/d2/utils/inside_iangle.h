#pragma once

#include "common/geometry/d2/angle_int.h"

namespace geometry {
namespace d2 {
// Check if tested angle between a1 and a2 if we assume clockwise direction.
template <class T>
constexpr bool Inside(const IAngle<T>& a1, const IAngle<T>& a2,
                      const IAngle<T>& atest) {
  const auto b2 = a2 - a1, bt = atest - a1;
  return !b2.Compare02Pi(bt);
}
}  // namespace d2
}  // namespace geometry
