#pragma once

#include "common/base.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

namespace geometry {
namespace d3 {
// Similar to Plane class but in point-normal form.
template <class T>
class PlanePN {
 public:
  using TPoint = Point<T>;
  using TVector = Vector<T>;

 public:
  TPoint p;
  TVector n;

 public:
  constexpr PlanePN() {}

  constexpr PlanePN(const Point<T>& _p, const Vector<T>& _n) : p(_p), n(_n) {}

  constexpr bool Valid() const { return !n.Empty(); }

  constexpr const Vector<T>& Normal() const { return n; }

  constexpr T operator()(const Point<T>& pp) const { return n * (pp - p); }

  constexpr void SetOppositeNormal() { n = -n; }

  constexpr void Normalize() { n.Normalize(); }
};
}  // namespace d3
}  // namespace geometry

using D3PlanePN = geometry::d3::PlanePN<double>;
using I3PlanePN = geometry::d3::PlanePN<int64_t>;
