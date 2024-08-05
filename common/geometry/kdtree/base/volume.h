#pragma once

namespace geometry {
namespace kdtree {
namespace base {
template <class TPoint>
constexpr auto Volume(const TPoint& l, const TPoint& r) {
  typename TPoint::T v(1);
  for (unsigned i = 0; i < l.Dim(); ++i) v *= (r[i] - l[i]);
  return v;
}
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
