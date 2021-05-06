#pragma once

#include "common/base.h"
#include "common/geometry/d3/point.h"

namespace geometry {
namespace kdtree {
template <class TValue>
inline TValue GetD(unsigned d, const geometry::d3::Point<TValue>& p) {
  assert(d < 3);
  return (d == 0) ? p.x : (d == 1) ? p.y : p.z;
}

template <class TValue>
inline void DSet(unsigned d, geometry::d3::Point<TValue>& p, const TValue& v) {
  assert(d < 3);
  ((d == 0) ? p.x : (d == 1) ? p.y : p.z) = v;
}

template <class TValue, class TPoint>
inline geometry::d3::Point<TValue> DSetCopy(
    unsigned d, const geometry::d3::Point<TValue>& p, const TValue& v) {
  assert(d < 3);
  return (d == 0) ? {v, p.y, p.z} : (d == 1) ? {p.x, v, p.z} : {p.x, p.y, v};
}
}  // namespace kdtree
}  // namespace geometry
