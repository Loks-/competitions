#pragma once

#include "common/base.h"
#include "common/geometry/d2/point.h"

namespace geometry {
namespace kdtree {
template <class TValue>
inline TValue DGet(unsigned d, const geometry::d2::Point<TValue>& p) {
  assert(d < 2);
  return (d == 0) ? p.x : p.y;
}

template <class TValue>
inline void DSet(unsigned d, geometry::d2::Point<TValue>& p, const TValue& v) {
  assert(d < 2);
  ((d == 0) ? p.x : p.y) = v;
}

template <class TValue, class TPoint>
inline geometry::d2::Point<TValue> DSetCopy(
    unsigned d, const geometry::d2::Point<TValue>& p, const TValue& v) {
  assert(d < 2);
  return (d == 0) ? {v, p.y} : {p.x, v};
}
}  // namespace kdtree
}  // namespace geometry
