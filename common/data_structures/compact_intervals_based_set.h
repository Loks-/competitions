#pragma once

#include "common/base.h"
#include <algorithm>
#include <vector>

// Use less memory compared to IntervalsBasedSet but Insert function requires
// O(n) instead O(log(n)) time.
namespace ds {
template <class TTValue = int64_t>
class CompactIntervalsBasedSet {
 public:
  using TValue = TTValue;
  using TSelf = CompactIntervalsBasedSet<TValue>;

  class Interval {
   public:
    TValue b, e;

    Interval() : b(0), e(0) {}
    explicit Interval(TValue v) : b(v), e(v + 1) {}
    Interval(TValue b_, TValue e_) : b(b_), e(e_) {}

    bool Empty() const { return e <= b; }
    TValue Size() const { return e - b; }
  };

 protected:
  std::vector<Interval> data;

 public:
  bool Empty() const { return data.empty(); }
  size_t VectorSize() const { return data.size(); }
  void Swap(TSelf& r) { data.swap(r.data); }

  bool HasKey(TValue key) const {
    auto it = std::lower_bound(
        data.begin(), data.end(), key,
        [](const Interval& i, TValue key) { return i.e <= key; });
    return (it != data.end()) && (it->b <= key);
  }

  void InsertBack(TValue key) { InsertBack(Interval(key)); }
  // void Insert(TValue key) { Insert(Interval(key)); }

  void InsertBack(const Interval& i) {
    if (data.empty() || (data.back().e < i.b)) {
      data.push_back(i);
    } else {
      assert(data.back().b <= i.b);
      data.back().e = std::max(data.back().e, i.e);
    }
  }

  // void Insert(const Interval& i) {}

  static TSelf Merge(const TSelf& l, const TSelf& r) {
    size_t i = 0, j = 0;
    TSelf o;
    while ((i < l.VectorSize()) && (j < r.VectorSize())) {
      if (l.data[i].b < r.data[j].b)
        o.InsertBack(l.data[i++]);
      else
        o.InsertBack(r.data[j++]);
    }
    while (i < l.VectorSize()) o.InsertBack(l.data[i++]);
    while (j < r.VectorSize()) o.InsertBack(r.data[j++]);
    return o;
  }

  TSelf Merge(const TSelf& r) const { return Merge(*this, r); }

  void Insert(const TSelf& cibs) {
    auto m = Merge(cibs);
    Swap(m);
  }

  void ShiftValues(TValue shift) {
    for (auto& i : data) {
      i.b += shift;
      i.e += shift;
    }
  }
};
}  // namespace ds
