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
  Point<T> p;
  Vector<T> n;

  PlanePN() {}
  PlanePN(const Point<T>& _p, const Vector<T>& _n) : p(_p), n(_n) {}

  bool Valid() const { return !n.Empty(); }
  const Vector<T>& Normal() const { return n; }
  T operator()(const Point<T>& pp) const { return n * (pp - p); }

  void SetOppositeNormal() { n = -n; }
  void Normalize() { n.Normalize(); }
};
}  // namespace d3
}  // namespace geometry

using D3PlanePN = geometry::d3::PlanePN<double>;
using I3PlanePN = geometry::d3::PlanePN<int64_t>;
