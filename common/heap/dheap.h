#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace heap {
template <unsigned d_, class TTData, class TTCompare = std::less<TTData>>
class DHeap {
 public:
  const unsigned d = d_;
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf = DHeap<d_, TData, TCompare>;

 protected:
  TCompare compare;
  std::vector<TData> data;

 public:
  DHeap() {}
  DHeap(unsigned expected_size) { data.reserve(expected_size); }
  DHeap(const std::vector<TData>& v) : data(v) { Heapify(); }

  bool Empty() const { return data.empty(); }
  unsigned Size() const { return unsigned(data.size()); }

  void Add(const TData& value) {
    data.push_back(value);
    SiftUp(unsigned(Size() - 1));
  }

  const TData& Top() const { return data[0]; }

  void Pop() {
    data[0] = data.back();
    data.pop_back();
    SiftDown(0);
  }

  TData Extract() {
    TData t = Top();
    Pop();
    return t;
  }

 protected:
  void SiftUp(unsigned pos) {
    if (pos == 0) return;
    unsigned npos = (pos - 1) / d;
    if (compare(data[pos], data[npos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / d;
        if (compare(x, data[npos]))
          data[pos] = data[npos];
        else
          break;
      }
      data[pos] = x;
    }
  }

  void SiftDown(unsigned pos) {
    unsigned cb = d * pos + 1, ce = std::min(cb + d, Size());
    if (ce <= cb) return;
    unsigned npos = cb;
    for (unsigned i = cb + 1; i < ce; ++i) {
      if (compare(data[i], data[npos])) npos = i;
    }
    if (compare(data[npos], data[pos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      for (pos = npos;; pos = npos) {
        cb = d * pos + 1, ce = std::min(cb + d, Size());
        if (ce <= cb) break;
        npos = cb;
        for (unsigned i = cb + 1; i < ce; ++i) {
          if (compare(data[i], data[npos])) npos = i;
        }
        if (compare(data[npos], x))
          data[pos] = data[npos];
        else
          break;
      }
      data[pos] = x;
    }
  }

  void Heapify() {
    for (unsigned pos = Size() / d; pos;) SiftDown(--pos);
  }

 public:
  void Union(const TSelf& r) {
    data.insert(data.end(), r.data.begin(), r.data.end());
    Heapify();
  }
};
}  // namespace heap
