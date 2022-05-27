#pragma once

#include "common/base.h"
#include "common/stl/pair.h"

#include <algorithm>
#include <vector>

namespace geometry {
namespace d1 {
template <class TValue>
class SegmentUnion {
 public:
  using T = TValue;

 protected:
  std::vector<std::pair<TValue, TValue>> vp;

 public:
  void Clear() { vp.clear(); }

  TValue Length() const {
    TValue s{};
    if (vp.empty()) return s;
    auto l = vp[0].first, r = l;
    for (auto& p : vp) {
      if (p.first <= r) {
        r = std::max(r, p.second);
      } else {
        s += r - l;
        l = p.first;
        r = p.second;
      }
    }
    s += r - l;
    return s;
  }

  void Add(const TValue& l, const TValue& r) {
    auto p = std::make_pair(l, r);
    auto it = std::lower_bound(vp.begin(), vp.end(), p);
    vp.insert(it, p);
  }

  void Remove(const TValue& l, const TValue& r) {
    auto p = std::make_pair(l, r);
    auto it = std::find(vp.begin(), vp.end(), p);
    vp.erase(it);
  }
};
}  // namespace d1
}  // namespace geometry
