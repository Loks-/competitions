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
  using TSelf = Polygon<T>;

  std::vector<TPoint> v;

  Polygon() {}
  explicit Polygon(const std::vector<TPoint>& vp) : v(vp) { Normalize(); }

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

  bool operator==(const TSelf& r) const { return v == r.v; }
  bool operator!=(const TSelf& r) const { return v != r.v; }

  unsigned Size() const { return v.size(); }

  const TPoint& operator[](unsigned i) const { return v[i]; }
  const TPoint& MGet(unsigned i) const { return v[i % v.size()]; }

  T DoubleArea() const {
    if (Size() < 3) return {};
    auto a = T();
    auto& p0 = v[0];
    for (unsigned i = 2; i < v.size(); ++i) a += (v[i - 1] - p0) % (v[i] - p0);
    return a;
  }

  T Area() const { return DoubleArea() / 2; }
};
}  // namespace d2
}  // namespace geometry

using D2Polygon = geometry::d2::Polygon<double>;
using I2Polygon = geometry::d2::Polygon<int64_t>;
