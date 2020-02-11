#pragma once

#include "common/geometry/d2/convex_polygon_static_size.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"
#include <array>

template <class T>
inline geometry::d2::ConvexPolygonStaticSize<T, 4> SquareFromDiagonal(
    const geometry::d2::Point<T>& a, const geometry::d2::Point<T>& c) {
  using TPoint = geometry::d2::Point<T>;
  using TVector = geometry::d2::Vector<T>;
  auto va = TVector(a), vc = TVector(c), vm2 = va + vc;
  geometry::d2::LinePV<T> l(a, c);
  auto vd = l.Normal();
  return geometry::d2::ConvexPolygonStaticSize<T, 4>(std::array<TPoint, 4>{
      a, ((vm2 + vd) / 2).ToPoint(), c, ((vm2 - vd) / 2).ToPoint()});
}
