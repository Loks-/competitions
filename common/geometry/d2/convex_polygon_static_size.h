#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/polygon_static_size.h"
#include "common/geometry/d2/triangle.h"

#include <algorithm>

namespace geometry {
namespace d2 {
template <class T, unsigned size>
class ConvexPolygonStaticSize : public PolygonStaticSize<T, size> {
 public:
  using TBase = PolygonStaticSize<T, size>;
  using TPoint = typename TBase::TPoint;

  explicit ConvexPolygonStaticSize(const std::array<TPoint, size>& vp)
      : TBase(vp) {}

  bool Inside(const TPoint& p) const {
    if (p == TBase::v[0]) return true;
    auto it = std::lower_bound(TBase::v.begin() + 1, TBase::v.end(), p,
                               [&](const TPoint& l, const TPoint& r) {
                                 return CompareVectorAngle(l - TBase::v[0],
                                                           r - TBase::v[0]);
                               });
    if (it == TBase::v.end()) return false;
    Triangle<T> t(TBase::v[0], *(it - 1), *it);
    return t.Inside(p);
  }
};
}  // namespace d2
}  // namespace geometry
