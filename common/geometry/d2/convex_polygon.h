#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/triangle_positive_area.h"
#include "common/geometry/d2/utils/has_point_segment.h"

#include <algorithm>

namespace geometry {
namespace d2 {
// Vertexes are saved in counterclockwise order
template <class T>
class ConvexPolygon : public Polygon<T> {
 public:
  using TBase = Polygon<T>;
  using TSelf = ConvexPolygon<T>;
  using TPoint = typename TBase::TPoint;

 public:
  constexpr ConvexPolygon() {}

  constexpr explicit ConvexPolygon(const std::vector<TPoint>& vp) : TBase(vp) {}

  constexpr ConvexPolygon(const std::vector<TPoint>& vp,
                          bool skip_normalization)
      : TBase(vp, skip_normalization) {}

  constexpr bool Inside(const TPoint& p) const {
    if (p == TBase::v[0]) return true;
    const auto it = std::lower_bound(TBase::v.begin() + 1, TBase::v.end(), p,
                                     [&](const TPoint& l, const TPoint& r) {
                                       return CompareVectorAngle(
                                           l - TBase::v[0], r - TBase::v[0]);
                                     });
    if (it == TBase::v.end()) return false;
    if (it == TBase::v.begin() + 1)
      return HasPoint(Segment<T, true>(TBase::v[0], TBase::v[1]), p);
    return TrianglePA<T>(TBase::v[0], *(it - 1), *it).Inside(p);
  }
};
}  // namespace d2
}  // namespace geometry

using D2CPolygon = geometry::d2::ConvexPolygon<double>;
using I2CPolygon = geometry::d2::ConvexPolygon<int64_t>;
