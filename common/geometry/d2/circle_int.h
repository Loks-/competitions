#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/distance.h"
#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
template <class T>
class ICircle {
 public:
  Point<T> c;
  T r, r2;

  ICircle() : r() { UpdateR2(); }
  ICircle(const Point<T>& _c, const T& _r) : c(_c), r(_r) { UpdateR2(); }
  ICircle(const Point<T>& _c, const T& _r, const T& _r2)
      : c(_c), r(_r), r2(_r2) {}

  void SetR2(const T& _r2) { r2 = _r2; }
  void UpdateR2() { SetR2(r * r); }

  bool Valid() const { return r2 >= 0; }

  bool Inside(const Point<T>& p) const { return SquaredDistance(p, c) <= r2; }

  T operator()(const Point<T>& p) const { return SquaredDistance(p, c) - r2; }
};
}  // namespace d2
}  // namespace geometry

using I2Circle = geometry::d2::ICircle<int64_t>;
