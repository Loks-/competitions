#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_yx.h"
#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/point.h"
#include "common/vector/rotate.h"

#include <algorithm>
#include <array>
#include <vector>

namespace geometry {
namespace d2 {
// Vertexes are saved in counterclockwise order
template <class T, unsigned size>
class PolygonStaticSize {
 public:
  using TPoint = Point<T>;
  std::array<TPoint, size> v;

  explicit PolygonStaticSize(const std::array<TPoint, size>& vp) : v(vp) {
    Normalize();
  }

  explicit PolygonStaticSize(const std::vector<TPoint>& vp)
      : v(vp.begin(), vp.end()) {
    Normalize();
  }

  void Normalize() {
    unsigned ilowest = 0;
    for (unsigned i = 1; i < size; ++i) {
      if (CompareYX(v[i], v[ilowest])) ilowest = i;
    }
    if (ilowest > 0)
      nvector::SwapIntervals(v.begin(), v.begin() + ilowest, v.end());
    if ((size > 2) && CompareVectorAngle(v.back() - v[0], v[1] - v[0]))
      std::reverse(v.begin() + 1, v.end());
  }

  unsigned Size() const { return size; }

  const TPoint& operator[](unsigned i) const { return v[i]; }
  const TPoint& MGet(unsigned i) const { return v[i % size]; }
};
}  // namespace d2
}  // namespace geometry
