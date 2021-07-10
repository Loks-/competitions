#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_yx.h"
#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/point.h"
#include "common/vector/rotate.h"

#include <algorithm>
#include <vector>

namespace geometry {
namespace d2 {
// Vertexes are saved in counterclockwise order
template <class T>
class Polygon {
 public:
  using TPoint = Point<T>;
  std::vector<TPoint> v;

  Polygon() {}
  explicit Polygon(const std::vector<TPoint>& vp) : v(vp) { Normalize(); }
  Polygon(const Polygon& p) : v(p.v) {}

  void Normalize() {
    unsigned ilowest = 0;
    for (unsigned i = 1; i < v.size(); ++i) {
      if (CompareYX(v[i], v[ilowest])) ilowest = i;
    }
    if (ilowest > 0)
      nvector::SwapIntervals(v.begin(), v.begin() + ilowest, v.end());
    if ((v.size() > 2) && CompareVectorAngle(v.back() - v[0], v[1] - v[0]))
      std::reverse(v.begin() + 1, v.end());
  }

  unsigned Size() const { return v.size(); }

  const TPoint& operator[](unsigned i) const { return v[i]; }
  const TPoint& MGet(unsigned i) const { return v[i % v.size()]; }
};
}  // namespace d2
}  // namespace geometry

using D2Polygon = geometry::d2::Polygon<double>;
using I2Polygon = geometry::d2::Polygon<int64_t>;
