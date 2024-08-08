#pragma once

#include "common/geometry/d1/segment_union1.h"
#include "common/geometry/d2/axis/rectangle.h"
#include "common/stl/pair.h"
#include "common/vector/unique.h"

#include <algorithm>
#include <utility>
#include <vector>

// Output area covered once and at least twice.
template <class T>
inline std::pair<T, T> UnionArea1(
    const std::vector<geometry::d2::axis::Rectangle<T>>& v) {
  if (v.empty()) return {};
  if (v.size() == 1) return {v[0].Area(), {}};
  std::vector<std::pair<T, unsigned>> vxm;
  std::vector<T> vy;
  for (unsigned i = 0; i < v.size(); ++i) {
    vxm.push_back({v[i].p1.x, 2 * i});
    vxm.push_back({v[i].p2.x, 2 * i + 1});
    vy.push_back(v[i].p1.y);
    vy.push_back(v[i].p2.y);
  }
  std::sort(vxm.begin(), vxm.end());
  nvector::UniqueUnsorted(vy);

  T s1{}, s2{};
  geometry::d1::SegmentUnion1<T> su;
  su.Init(vy);
  T x = vxm[0].first;
  for (auto& p : vxm) {
    if (x < p.first) {
      s1 += (p.first - x) * su.Length1();
      s2 += (p.first - x) * su.Length2Plus();
      x = p.first;
    }
    const auto& r = v[p.second / 2];
    if (p.second & 1) {
      su.Remove(r.p1.y, r.p2.y);
    } else {
      su.Add(r.p1.y, r.p2.y);
    }
  }
  return {s1, s2};
}
