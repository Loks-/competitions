#pragma once

#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
template <class T, bool closed>
class Segment {
 public:
  using TPoint = Point<T>;
  using TSelf = Segment<T, closed>;

 public:
  TPoint p1, p2;

 public:
  static consteval bool Closed() { return closed; }

  constexpr Segment(const TPoint& _p1, const TPoint& _p2) : p1(_p1), p2(_p2) {}

  constexpr bool Empty() const { return p1 == p2; }

  constexpr bool operator==(const TSelf& r) const {
    return (p1 == r.p1) && (p2 == r.p2);
  }

  constexpr TSelf Flip() const { return {p2, p1}; }
};
}  // namespace d2
}  // namespace geometry

using D2ClosedSegment = geometry::d2::Segment<double, true>;
using D2OpenSegment = geometry::d2::Segment<double, false>;
using I2ClosedSegment = geometry::d2::Segment<int64_t, true>;
using I2OpenSegment = geometry::d2::Segment<int64_t, false>;
