#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/polygon_static_size.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/triangle_positive_area.h"
#include "common/geometry/d2/utils/has_point_segment.h"

#include <algorithm>

namespace geometry {
namespace d2 {
template <class T, unsigned size>
class ConvexPolygonStaticSize : public PolygonStaticSize<T, size> {
 public:
  using TBase = PolygonStaticSize<T, size>;
  using TPoint = typename TBase::TPoint;

  constexpr explicit ConvexPolygonStaticSize(const std::array<TPoint, size>& vp)
      : TBase(vp) {}

  constexpr ConvexPolygonStaticSize(const std::array<TPoint, size>& vp,
                                    bool skip_normalization)
      : TBase(vp, skip_normalization) {}

  constexpr bool Inside(const TPoint& p) const {
    if (p == TBase::v[0]) return true;
    auto it = std::lower_bound(TBase::v.begin() + 1, TBase::v.end(), p,
                               [&](const TPoint& l, const TPoint& r) {
                                 return CompareVectorAngle(l - TBase::v[0],
                                                           r - TBase::v[0]);
                               });
    if (it == TBase::v.end()) return false;
    if (it == TBase::v.begin() + 1)
      return HasPoint(Segment<T, true>(TBase::v[0], TBase::v[1]), p);
    TrianglePA<T> t(TBase::v[0], *(it - 1), *it);
    return t.Inside(p);
  }
};
}  // namespace d2
}  // namespace geometry
