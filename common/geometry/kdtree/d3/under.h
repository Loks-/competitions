#pragma once

#include "common/geometry/d3/point.h"

namespace geometry {
namespace kdtree {
namespace base {
template <class TTValue>
inline bool Under(const geometry::d3::Point<TTValue>& l,
                  const geometry::d3::Point<TTValue>& r) {
  return (l.x <= r.x) && (l.y <= r.y) && (l.z <= r.z);
}

template <class TTValue>
inline bool StrictUnder(const geometry::d3::Point<TTValue>& l,
                        const geometry::d3::Point<TTValue>& r) {
  return (l.x < r.x) && (l.y < r.y) && (l.z < r.z);
}
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
