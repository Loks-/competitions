#pragma once

namespace geometry {
namespace kdtree {
namespace base {
template <class TPoint, class TValue>
inline TPoint Shift(const TPoint& p, const TValue& value) {
  TPoint pnew;
  for (unsigned d = 0; d < p.dim; ++d) pnew[d] = p[d] + value;
  return pnew;
}
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
