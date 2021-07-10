#pragma once

#include "common/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"
#include "common/vector/unique.h"

#include <algorithm>
#include <vector>

namespace geometry {
namespace d2 {
namespace axis {
// Polygon with edges parallel to coordinate axis
template <class T>
class Polygon : public geometry::d2::Polygon<T> {
 public:
  using TBase = geometry::d2::Polygon<T>;
  using TPoint = typename TBase::TPoint;

 protected:
  std::vector<T> vx;
  std::vector<std::vector<T>> vvy;

 public:
  explicit Polygon(const std::vector<TPoint>& vp) : TBase(vp) { Normalize(); }

  void Normalize() {
    TBase::Normalize();
    vx.clear();
    vvy.clear();
    for (auto& p : TBase::v) vx.push_back(p.x);
    nvector::UniqueS(vx);
    vvy.resize(vx.size());
    for (unsigned i = 0; i < TBase::Size(); ++i) {
      auto p0 = TBase::v[i], p1 = TBase::MGet(i + 1);
      assert((i & 1) ? p0.x == p1.x : p0.y == p1.y);
      if (p0.x != p1.x) {
        auto i0 = std::lower_bound(vx.begin(), vx.end(), p0.x) - vx.begin();
        auto i1 = std::lower_bound(vx.begin(), vx.end(), p1.x) - vx.begin();
        if (i0 > i1) std::swap(i0, i1);
        for (auto j = i0; j < i1; ++j) vvy[j].push_back(p0.y);
      }
    }
    for (auto& vy : vvy) std::sort(vy.begin(), vy.end());
  }

  bool Inside(const Point<T>& p) const {
    auto itx = std::lower_bound(vx.begin(), vx.end(), p.x);
    if (itx == vx.end()) return false;
    for (unsigned i = 0, j = itx - vx.begin(); i < 2; ++i) {
      if ((i == 0) && (p.x < vx[j])) continue;
      if ((i == 1) && (j == 0)) continue;
      const auto& vy = vvy[j - i];
      auto ity = std::lower_bound(vy.begin(), vy.end(), p.y);
      if ((ity != vy.end()) && ((p.y == *ity) || ((ity - vy.begin()) & 1)))
        return true;
    }
    return false;
  }
};
}  // namespace axis
}  // namespace d2
}  // namespace geometry

using D2APolygon = geometry::d2::axis::Polygon<double>;
using I2APolygon = geometry::d2::axis::Polygon<int64_t>;
