#pragma once

#include "common/base.h"
#include <functional>
#include <vector>

namespace heap {
template <class TTValue, class TTCompare = std::less<TTValue>>
class UKeyValueHeap {
 public:
  const unsigned not_in_heap = unsigned(-1);
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TSelf = UKeyValueHeap<TValue, TCompare>;

  struct TData {
    unsigned key;
    TValue value;
  };

 protected:
  TCompare compare;
  std::vector<TData> data;
  std::vector<unsigned> heap_position;

 protected:
  bool Compare(const TData& l, const TData& r) {
    return compare(l.value, r.value);
  }

  void ResetHeapPosition(unsigned ukey_size) {
    heap_position.clear();
    heap_position.resize(ukey_size, not_in_heap);
  }

 public:
  UKeyValueHeap(unsigned ukey_size) {
    ResetHeapPosition(ukey_size);
    data.reserve(ukey_size);
  }

  UKeyValueHeap(unsigned ukey_size, const std::vector<TData>& v) : data(v) {
    ResetHeapPosition(ukey_size);
    Heapify();
  }

  bool Empty() const { return data.empty(); }
  size_t Size() const { return data.size(); }
  unsigned UKeySize() const { return unsigned(heap_position.size()); }

  TValue Get(unsigned key) const {
    unsigned key_position = heap_position[key];
    assert(key_position != not_in_heap);
    return data[key_position];
  }

  void Set(unsigned key, const TValue& new_value) {
    unsigned key_position = heap_position[key];
    if (key_position == not_in_heap) {
      heap_position[key] = data.size();
      data.push_back({key, new_value});
      SiftUp(heap_position[key]);
    } else if (compare(new_value, data[key_position].value)) {
      data[key_position].value = new_value;
      SiftUp(key_position);
    } else {
      data[key_position].value = new_value;
      SiftDown(key_position);
    }
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  const TData& Top() const { return data[0]; }

  void Pop() {
    heap_position[Top().key] = not_in_heap;
    if (Size() > 1) {
      data[0] = data.back();
      data.pop_back();
      heap_position[Top().key] = 0;
      SiftDown(0);
    } else {
      data.pop_back();
    }
  }

  TData Extract() {
    TData t = Top();
    Pop();
    return t;
  }

  void SiftUp(unsigned pos) {
    if (pos == 0) return;
    unsigned npos = (pos - 1) / 2;
    if (Compare(data[pos], data[npos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      heap_position[data[pos].key] = pos;
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / 2;
        if (Compare(x, data[npos])) {
          data[pos] = data[npos];
          heap_position[data[pos].key] = pos;
        } else {
          break;
        }
      }
      data[pos] = x;
      heap_position[x.key] = pos;
    }
  }

  void SiftDown(unsigned pos) {
    unsigned npos = 2 * pos + 1;
    if (npos >= data.size()) return;
    if ((npos + 1 < data.size()) && Compare(data[npos + 1], data[npos])) ++npos;
    if (Compare(data[npos], data[pos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      heap_position[data[pos].key] = pos;
      for (pos = npos;; pos = npos) {
        npos = 2 * pos + 1;
        if (npos >= data.size()) break;
        if ((npos + 1 < data.size()) && Compare(data[npos + 1], data[npos]))
          ++npos;
        if (Compare(data[npos], x)) {
          data[pos] = data[npos];
          heap_position[data[pos].key] = pos;
        } else {
          break;
        }
      }
      data[pos] = x;
      heap_position[x.key] = pos;
    }
  }

  void Heapify() {
    for (unsigned pos = 0; pos < data.size(); ++pos)
      heap_position[data[pos].key] = pos;
    for (unsigned pos = data.size() / 2; pos;) SiftDown(--pos);
  }
};
}  // namespace heap
