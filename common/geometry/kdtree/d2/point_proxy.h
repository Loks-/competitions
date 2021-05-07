#pragma once

#include "common/geometry/d2/point.h"
#include "common/geometry/kdtree/point_proxy.h"

#include <algorithm>

namespace geometry {
namespace kdtree {
template <class TTValue>
class PointProxy<geometry::d2::Point<TTValue>> {
 public:
  using TValue = TTValue;
  using TPoint = geometry::d2::Point<TValue>;

  static TValue DGet(unsigned d, const TPoint& p) {
    assert(d < 2);
    return (d == 0) ? p.x : p.y;
  }

  static void DSet(unsigned d, TPoint& p, const TValue& v) {
    assert(d < 2);
    ((d == 0) ? p.x : p.y) = v;
  }

  static TPoint DSetCopy(unsigned d, const TPoint& p, const TValue& v) {
    assert(d < 2);
    return (d == 0) ? TPoint(v, p.y) : TPoint(p.x, v);
  }

  static TPoint DMin(const TPoint& l, const TPoint& r) {
    return {std::min(l.x, r.x), std::min(l.y, r.y)};
  }

  static TPoint DMax(const TPoint& l, const TPoint& r) {
    return {std::max(l.x, r.x), std::max(l.y, r.y)};
  }

  static bool Under(const TPoint& l, const TPoint& r) {
    return (l.x <= r.x) && (l.y <= r.y);
  }

  static bool StrictUnder(const TPoint& l, const TPoint& r) {
    return (l.x < r.x) && (l.y < r.y);
  }
};
}  // namespace kdtree
}  // namespace geometry
