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
  using T = TValue;
  using TSegment = std::pair<T, T>;
  using TVector = std::vector<TSegment>;

 protected:
  TVector vp;

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

  static TVector Compress(const TVector& v) {
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
inline std::vector<std::pair<TValue, TValue>> SegmentMerge(
    const std::vector<std::pair<TValue, TValue>>& v) {
  return geometry::d1::SegmentUnionBase<TValue>::Compress(v);
}
