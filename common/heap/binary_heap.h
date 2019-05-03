#pragma once

#include <functional>
#include <vector>

namespace heap {
template <class TTData, class TTCompare = std::less<TTData>>
class BinaryHeap {
 public:
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf = BinaryHeap<TData, TCompare>;

 protected:
  TCompare compare;
  std::vector<TData> data;

 public:
  BinaryHeap() {}
  BinaryHeap(unsigned expected_size) { data.reserve(expected_size); }
  BinaryHeap(const std::vector<TData>& v) : data(v) { Heapify(); }

  bool Empty() const { return data.empty(); }
  size_t Size() const { return data.size(); }

  void Add(const TData& value) {
    data.push_back(value);
    SiftUp(unsigned(data.size() - 1));
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

  void SiftUp(unsigned pos) {
    if (pos == 0) return;
    unsigned npos = (pos - 1) / 2;
    if (compare(data[pos], data[npos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / 2;
        if (compare(x, data[npos])) {
          data[pos] = data[npos];
        } else {
          break;
        }
      }
      data[pos] = x;
    }
  }

  void SiftDown(unsigned pos) {
    unsigned npos = 2 * pos + 1;
    if (npos >= data.size()) return;
    if ((npos + 1 < data.size()) && compare(data[npos + 1], data[npos])) ++npos;
    if (compare(data[npos], data[pos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      for (pos = npos;; pos = npos) {
        npos = 2 * pos + 1;
        if (npos >= data.size()) break;
        if ((npos + 1 < data.size()) && compare(data[npos + 1], data[npos]))
          ++npos;
        if (compare(data[npos], x)) {
          data[pos] = data[npos];
        } else {
          break;
        }
      }
      data[pos] = x;
    }
  }

  void Heapify() {
    for (unsigned pos = data.size() / 2; pos;) SiftDown(--pos);
  }

  void Union(const TSelf& r) {
    data.insert(data.end(), r.data.begin(), r.data.end());
    Heapify();
  }
};
}  // namespace heap
