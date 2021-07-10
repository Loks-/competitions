#pragma once

#include "common/geometry/d2/axis/polygon.h"

template <class T1, class TPointSet>
inline typename TPointSet::TWeight Count(
    const TPointSet& ps, const geometry::d2::axis::Polygon<T1>& p) {
  typename TPointSet::TWeight s0 = 0, s1 = 0;
  auto p0 = p[p.Size() - 1], p1 = p[0], p2 = p[1];
  for (unsigned i = 0; i < p.Size(); ++i) {
    unsigned ix = (p0.y < p1.y) || (p2.y < p1.y),
             iy = (p0.x < p1.x) || (p2.x < p1.x), i1 = (i & 1);
    (i1 ? s0 : s1) += ps.CountQ({p1.x + (ix ^ i1), p1.y + (iy ^ i1)});
    p0 = p1;
    p1 = p2;
    p2 = p.MGet(i + 2);
  }
  return s1 - s0;
}
