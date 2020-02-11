#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
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

  PolygonStaticSize(const std::array<TPoint, size>& vp) : v(vp) { Normalize(); }

  PolygonStaticSize(const std::vector<TPoint>& vp) : v(vp.begin(), vp.end()) {
    Normalize();
  }

  void Normalize() {
    unsigned ilowest = 0;
    for (unsigned i = 1; i < size; ++i) {
      if ((v[i].y < v[ilowest].y) ||
          ((v[i].y == v[ilowest].y) && (v[i].x < v[ilowest].x)))
        ilowest = i;
    }
    SwapIntervals(v.begin(), v.begin() + ilowest, v.end());
    if ((size > 2) && CompareVectorAngle(v.back() - v[0], v[1] - v[0]))
      std::reverse(v.begin() + 1, v.end());
  }
};
}  // namespace d2
}  // namespace geometry
