#pragma once

namespace geometry {
namespace kdtree {
namespace base {
template <class TPoint, class TValue>
inline TPoint DSet(unsigned d, const TPoint& p, const TValue& value) {
  TPoint pnew(p);
  pnew[d] = value;
  return pnew;
}
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
