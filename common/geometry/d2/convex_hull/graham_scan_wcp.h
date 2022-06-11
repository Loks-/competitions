#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/point_yx.h"
#include "common/geometry/d2/convex_polygon.h"
#include "common/geometry/d2/point.h"

#include <algorithm>
#include <vector>

namespace geometry {
namespace d2 {
namespace convex_hull {
template <class T>
inline ConvexPolygon<T> GrahamScanWCP(const std::vector<Point<T>>& vpoints) {
  if (vpoints.size() <= 3) return ConvexPolygon<T>(vpoints);
  std::vector<Point<T>> v(vpoints), vh(v.size());
  unsigned ilowest = 0;
  for (unsigned i = 1; i < v.size(); ++i) {
    if (CompareYX(v[i], v[ilowest])) ilowest = i;
  }
  if (ilowest > 0) std::swap(v[0], v[ilowest]);
  std::sort(v.begin() + 1, v.end(),
            [&](const Point<T>& p1, const Point<T>& p2) {
              return (p2 - v[0]) % (p1 - v[0]) < 0;
            });

  vh[0] = v[0];
  unsigned i = 1, j = 0;
  for (; i < v.size(); ++i) {
    for (; (j > 1) && (((vh[j - 1] - vh[j]) % (v[i] - vh[j])) >= 0);) --j;
    vh[++j] = v[i];
  }
  vh.resize(j + 1);
  return ConvexPolygon<T>(vh, true);
}
}  // namespace convex_hull
}  // namespace d2
}  // namespace geometry
