#pragma once

#include "common/geometry/d2/axis/points_set.h"
#include "common/geometry/d2/axis/polygon.h"

template <class T1, class T2>
inline T2 Count(const geometry::d2::axis::PointsSet<T1, T2>& ps,
                const geometry::d2::axis::Polygon<T1>& p) {
  T2 s0 = 0, s1 = 0;
  auto p0 = p[p.Size() - 1], p1 = p[0], p2 = p[1];
  for (unsigned i = 0; i < p.Size(); ++i) {
    bool ix = (p0.y < p1.y) || (p2.y < p1.y),
         iy = (p0.x < p1.x) || (p2.x < p1.x), i1 = (i & 1);
    (i1 ? s0 : s1) += ps.CountQ(p1, ix != i1, iy != i1);
    p0 = p1;
    p1 = p2;
    p2 = p[(i + 2) % p.Size()];
  }
  return s1 - s0;
}
