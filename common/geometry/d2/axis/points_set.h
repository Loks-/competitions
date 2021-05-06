#pragma once

#include "common/base.h"
#include "common/binary_indexed_tree/bit_2d.h"
#include "common/data_structures/coordinate_compression.h"
#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/point.h"

#include <vector>

namespace geometry {
namespace d2 {
namespace axis {
template <class T1, class T2 = T1>
class PointsSet {
 public:
  using TPoint = Point<T1>;

 protected:
  ds::CoordinateCompression<T1> ccx, ccy;
  BIT_2D<T2> s;

 public:
  PointsSet() {}

  PointsSet(const std::vector<TPoint>& points) { Init(points); }

  void Init(const std::vector<TPoint>& points) {
    std::vector<T1> vx, vy;
    for (auto& p : points) {
      vx.push_back(p.x);
      vy.push_back(p.y);
    }
    ccx.InitUnsorted(vx);
    ccy.InitUnsorted(vy);
    s.Reset(ccx.Size() + 1, ccy.Size() + 1);
    for (auto& p : points) s.Add(ccx.GetNew(p.x), ccy.GetNew(p.y));
  }

  T2 CountQ(const TPoint& p, bool include_x, bool include_y) const {
    auto x = include_x ? ccx.UpperBound(p.x) : ccx.LowerBound(p.x),
         y = include_y ? ccy.UpperBound(p.y) : ccy.LowerBound(p.y);
    return s.Sum(x, y);
  }

  T2 CountQ0(const TPoint& p) const { return CountQ(p, false, false); }

  T2 CountQ1(const TPoint& p) const { return CountQ(p, true, true); }

  T2 Count(const TPoint& p) const {
    auto x0 = ccx.LowerBound(p.x), x1 = ccx.UpperBound(p.x),
         y0 = ccy.LowerBound(p.y), y1 = ccy.UpperBound(p.y);
    return ((x0 == x1) || (y0 == y1)) ? T2(0) : s.Sum(x0, y0, x1, y1);
  }

  T2 Count(const Rectangle<T1>& r) const {
    auto x0 = ccx.LowerBound(r.p1.x), x1 = ccx.UpperBound(r.p2.x),
         y0 = ccy.LowerBound(r.p1.y), y1 = ccy.UpperBound(r.p2.y);
    return ((x0 == x1) || (y0 == y1)) ? T2(0) : s.Sum(x0, y0, x1, y1);
  }
};
}  // namespace axis
}  // namespace d2
}  // namespace geometry

using I2APointsSet = geometry::d2::axis::PointsSet<int64_t>;
