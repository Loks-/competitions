#pragma once

#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
template <class T, bool closed>
class Segment {
 public:
  Point<T> p1, p2;

 public:
  Segment(const Point<T>& _p1, const Point<T>& _p2) : p1(_p1), p2(_p2) {}

  bool Empty() const { return p1 == p2; }
  bool operator==(const Segment<T, closed>& r) const { return (p1 == r.p1) && (p2 == r.p2); }
};
}  // namespace d2
}  // namespace geometry

using D2ClosedSegment = geometry::d2::Segment<double, true>;
using D2OpenSegment = geometry::d2::Segment<double, false>;
using I2ClosedSegment = geometry::d2::Segment<int64_t, true>;
using I2OpenSegment = geometry::d2::Segment<int64_t, false>;
