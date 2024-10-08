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

 protected:
  std::array<TPoint, size> v;

 public:
  constexpr explicit PolygonStaticSize(const std::array<TPoint, size>& vp)
      : v(vp) {
    Normalize();
  }

  constexpr explicit PolygonStaticSize(const std::vector<TPoint>& vp)
      : v(vp.begin(), vp.end()) {
    Normalize();
  }

  constexpr PolygonStaticSize(const std::array<TPoint, size>& vp,
                              bool skip_normalization)
      : v(vp) {
    if (!skip_normalization) Normalize();
  }

  constexpr void Normalize() {
    unsigned ilowest = 0;
    for (unsigned i = 1; i < size; ++i) {
      if (CompareYX(v[i], v[ilowest])) ilowest = i;
    }
    if (ilowest > 0)
      nvector::SwapIntervals(v.begin(), v.begin() + ilowest, v.end());
    if ((size > 2) && CompareVectorAngle(v.back() - v[0], v[1] - v[0]))
      std::reverse(v.begin() + 1, v.end());
  }

  static consteval unsigned Size() { return size; }

  constexpr const std::array<TPoint, size>& Vertices() const { return v; }

  constexpr const TPoint& operator[](unsigned i) const { return v[i]; }

  constexpr const TPoint& MGet(unsigned i) const { return v[i % size]; }
};
}  // namespace d2
}  // namespace geometry
