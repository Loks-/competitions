#pragma once

#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/reflection/points.h"
#include "common/geometry/d2/utils/midpoint.h"

namespace geometry {
namespace d2 {
namespace reflection {
// Assume no angles are equal to pi.
template <class T>
inline bool Symmetric(const Polygon<T>& pl) {
  unsigned s = pl.Size();
  std::vector<Point<T>> vnew;
  for (unsigned i = 0; i < s; ++i) {
    auto &p1 = pl[i], &p2 = pl.MGet(i + 1);
    vnew.push_back({p1.x + p1.x, p1.y + p1.y});
    vnew.push_back({p1.x + p2.x, p1.y + p2.y});
  }
  Polygon<T> pnew(vnew);
  for (unsigned i = 0; i < s; ++i) {
    LinePV<T> l(pnew[i], pnew[i + s]);
    bool good = true;
    for (unsigned j = 1; j < s; ++j) {
      if (!Reflected(l, pnew[i + j], pnew.MGet(i + 2 * s - j))) {
        good = false;
        break;
      }
    }
    if (good) return true;
  }
  return false;
}

// Assume no angles are equal to pi.
// All coordinates should be even if T is integer.
template <class T>
inline LinePV<T> LineOfSymmetry(const Polygon<T>& pl) {
  unsigned s = pl.Size();
  std::vector<Point<T>> vnew;
  for (unsigned i = 0; i < s; ++i) {
    auto &p1 = pl[i], &p2 = pl.MGet(i + 1);
    vnew.push_back(p1);
    vnew.push_back(MidPoint(p1, p2));
  }
  Polygon<T> pnew(vnew);
  for (unsigned i = 0; i < s; ++i) {
    LinePV<T> l(pnew[i], pnew[i + s]);
    bool good = true;
    for (unsigned j = 1; j < s; ++j) {
      if (!Reflected(l, pnew[i + j], pnew.MGet(i + 2 * s - j))) {
        good = false;
        break;
      }
    }
    if (good) return l;
  }
  return {};
}
}  // namespace reflection
}  // namespace d2
}  // namespace geometry
