#pragma once

#include "common/base.h"
#include "common/stl/pair.h"

#include <algorithm>
#include <vector>

namespace geometry {
namespace d1 {
// Linear time to Add/Remove/Length but not requires to know possible values in
// advance.
template <class TValue>
class SegmentUnionBase {
 public:
  using TSegment = std::pair<TValue, TValue>;
  using TVector = std::vector<TSegment>;

 protected:
  TVector vp;

 public:
  constexpr void Clear() { vp.clear(); }

  constexpr TValue Length() const {
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

  constexpr void Add(const TValue& l, const TValue& r) {
    const auto p = std::make_pair(l, r);
    const auto it = std::lower_bound(vp.begin(), vp.end(), p);
    vp.insert(it, p);
  }

  constexpr void Remove(const TValue& l, const TValue& r) {
    const auto p = std::make_pair(l, r);
    const auto it = std::find(vp.begin(), vp.end(), p);
    vp.erase(it);
  }

  static constexpr TVector Compress(const TVector& v) {
    if (v.size() <= 1) return v;
    TVector v1(v);
    std::sort(v1.begin(), v1.end());
    unsigned i = 1, j = 0;
    for (; i < v1.size(); ++i) {
      if (v1[i].first <= v1[j].second) {
        v1[j].second = std::max(v1[j].second, v1[i].second);
      } else {
        v1[++j] = v1[i];
      }
    }
    v1.resize(j + 1);
    return v1;
  }
};
}  // namespace d1
}  // namespace geometry

template <class TValue>
constexpr std::vector<std::pair<TValue, TValue>> SegmentMerge(
    const std::vector<std::pair<TValue, TValue>>& v) {
  return geometry::d1::SegmentUnionBase<TValue>::Compress(v);
}
