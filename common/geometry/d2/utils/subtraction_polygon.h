#pragma once

#include "common/geometry/d2/location/location.h"
#include "common/geometry/d2/location/point_polygon.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"

#include <vector>

namespace geometry {
namespace d2 {
// Assume small polygon is fully inside large polygon and have common points on
// border.
// Return empty vector if plgn_large == plgn_small or subtraction is impossible.
template <class T>
inline std::vector<Polygon<T>> Subtraction(const Polygon<T>& plgn_large,
                                           const Polygon<T>& plgn_small) {
  if (plgn_large == plgn_small) return {};
  std::vector<location::Location> vl;
  for (auto& p : plgn_small.v) {
    vl.push_back(location::Locate(p, plgn_large));
    if (vl.back().type == location::Location::OUTSIDE) return {};
  }
  std::vector<Point<T>> vp;
  std::vector<Polygon<T>> vr;
  for (unsigned i = 0, j; i < vl.size(); i = j) {
    if (vl[i].type == location::Location::INSIDE) continue;
    for (j = i + 1; vl[j % vl.size()].type == location::Location::INSIDE;) ++j;
    if (j == i + vl.size()) continue;
    auto &p1 = plgn_small[i], &p2 = plgn_small.MGet(j);
    auto &l1 = vl[i], &l2 = vl[j % vl.size()];
    unsigned l2a = (l2.type == location::Location::VERTEX)
                       ? (l2.index == 0) ? plgn_large.Size() - 1 : l2.index - 1
                       : l2.index;
    if (l2a < l1.index) l2a += plgn_large.Size();
    if ((j == i + 1) && (l2a == l1.index)) continue;  // Empty polygon
    vp.clear();
    vp.push_back(p1);
    for (unsigned k = l1.index + 1; k <= l2a; ++k)
      vp.push_back(plgn_large.MGet(k));
    vp.push_back(p2);
    for (unsigned k = j - 1; k > i; --k) vp.push_back(plgn_small.MGet(k));
    vr.push_back(Polygon<T>(vp));
  }
  return vr;
}
}  // namespace d2
}  // namespace geometry
