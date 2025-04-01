#pragma once

namespace geometry {
namespace kdtree {
namespace base {
template <class TPoint>
constexpr bool Under(const TPoint& l, const TPoint& r) {
  for (unsigned i = 0; i < TPoint::Dim(); ++i) {
    if (l[i] > r[i]) return false;
  }
  return true;
}

template <class TPoint>
constexpr bool StrictUnder(const TPoint& l, const TPoint& r) {
  for (unsigned i = 0; i < TPoint::Dim(); ++i) {
    if (l[i] >= r[i]) return false;
  }
  return true;
}
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
