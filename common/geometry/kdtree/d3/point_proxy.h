#pragma once

#include "common/geometry/d3/point.h"
#include "common/geometry/kdtree/point_proxy.h"

namespace geometry {
namespace kdtree {
template <class TTValue>
class PointProxy<geometry::d3::Point<TTValue>> {
 public:
  using TValue = TTValue;
  using TPoint = geometry::d3::Point<TValue>;

  static TValue DGet(unsigned d, const TPoint& p) {
    assert(d < 3);
    return (d == 0) ? p.x : (d == 1) ? p.y : p.z;
  }

  static void DSet(unsigned d, TPoint& p, const TValue& v) {
    assert(d < 3);
    ((d == 0) ? p.x : (d == 1) ? p.y : p.z) = v;
  }

  static TPoint DSetCopy(unsigned d, const TPoint& p, const TValue& v) {
    assert(d < 3);
    return (d == 0) ? TPoint(v, p.y, p.z) : (d == 1) ? TPoint(p.x, v, p.z)
                                                     : TPoint(p.x, p.y, v);
  }

  static bool Under(const TPoint& l, const TPoint& r) {
    return (l.x <= r.x) && (l.y <= r.y) && (l.z <= r.z);
  }

  static bool StrictUnder(const TPoint& l, const TPoint& r) {
    return (l.x < r.x) && (l.y < r.y) && (l.z < r.z);
  }
};
}  // namespace kdtree
}  // namespace geometry
