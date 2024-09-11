#pragma once

#include <functional>
#include <vector>

namespace heap {
namespace base {
// Memory  -- O(N)
// Add     -- O(log N / log d)
// Top     -- O(1)
// Pop     -- O(d log N / log d)
// Init    -- O(N)
// Union   -- O(N)
template <unsigned d, class TData, class TCompare = std::less<TData>>
class DHeap {
 public:
  using TSelf = DHeap<d, TData, TCompare>;

 protected:
  TCompare compare;
  std::vector<TData> data;

 public:
  constexpr DHeap() {}

  constexpr explicit DHeap(unsigned expected_size) {
    data.reserve(expected_size);
  }

  constexpr explicit DHeap(const std::vector<TData>& v) : data(v) { Heapify(); }

  constexpr bool Empty() const { return data.empty(); }

  constexpr unsigned Size() const { return unsigned(data.size()); }

  constexpr void Add(const TData& value) {
    data.push_back(value);
    SiftUp(unsigned(Size() - 1));
  }

  constexpr const TData& Top() const { return data[0]; }

  constexpr void Pop() {
    data[0] = data.back();
    data.pop_back();
    SiftDown(0);
  }

  constexpr TData Extract() {
    const TData t = Top();
    Pop();
    return t;
  }

  constexpr void ExtractAll() { data.clear(); }

  constexpr void Union(const TSelf& r) {
    data.insert(data.end(), r.data.begin(), r.data.end());
    Heapify();
  }

 protected:
  constexpr void SiftUp(unsigned pos) {
    if (pos == 0) return;
    unsigned npos = (pos - 1) / d;
    if (compare(data[pos], data[npos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / d;
        if (compare(x, data[npos])) {
          data[pos] = data[npos];
        } else {
          break;
        }
      }
      data[pos] = x;
    }
  }

  constexpr bool SiftDownNext(const TData& x, unsigned pos,
                              unsigned& npos) const {
    const unsigned cb = d * pos + 1, ce = std::min(cb + d, Size());
    if (cb >= ce) return false;
    npos = cb;
    for (unsigned i = cb + 1; i < ce; ++i) {
      if (compare(data[i], data[npos])) npos = i;
    }
    return compare(data[npos], x);
  }

  constexpr void SiftDown(unsigned pos) {
    unsigned npos;
    TData x = data[pos];
    if (SiftDownNext(x, pos, npos)) {
      data[pos] = data[npos];
      for (pos = npos;; pos = npos) {
        if (SiftDownNext(x, pos, npos)) {
          data[pos] = data[npos];
        } else {
          break;
        }
      }
      data[pos] = x;
    }
  }

  constexpr void Heapify() {
    for (unsigned pos = Size() / d; pos;) SiftDown(--pos);
  }
};
}  // namespace base
}  // namespace heap
