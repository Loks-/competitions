#pragma once

#include "common/base.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/distance.h"
#include "common/geometry/d2/point.h"

namespace geometry {
namespace d2 {
// Ellipse in foci and distance form.
template <class T>
class Ellipse {
 public:
  Point<T> f1, f2;
  T a;

  Ellipse() : a() {}
  Ellipse(const Point<T>& _f1, const Point<T>& _f2, const T& _a)
      : f1(_f1), f2(_f2), a(_a) {}

  bool Valid() const { return Distance(f1, f2) <= 2 * a; }

  T operator()(const Point<T>& p) const {
    return Distance(p, f1) + Distance(p, f2) - 2 * a;
  }
};
}  // namespace d2
}  // namespace geometry

using D2Ellipse = geometry::d2::Ellipse<double>;
using I2Ellipse = geometry::d2::Ellipse<int64_t>;
