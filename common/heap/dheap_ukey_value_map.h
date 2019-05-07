#pragma once

#include "common/base.h"
#include "common/vector/enumerate.h"
#include <algorithm>
#include <functional>
#include <vector>

namespace heap {
// DHeap with map to position by key.
// Values are stored outside of heap.
template <unsigned d_, class TTValue, class TTCompare = std::less<TTValue>>
class DHeapUKeyValueMap {
 public:
  const unsigned d = d_;
  const unsigned not_in_heap = unsigned(-1);
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TSelf = DHeapUKeyValueMap<d_, TValue, TCompare>;

  struct TData {
    unsigned key;
    TValue value;
  };

 protected:
  TCompare compare;
  std::vector<unsigned> heap_keys;
  std::vector<unsigned> heap_position;
  std::vector<TValue> values;

 protected:
  bool Compare(unsigned lkey, unsigned rkey) const {
    return compare(values[lkey], values[rkey]);
  }

  void ResetHeapPosition(unsigned ukey_size) {
    heap_position.clear();
    heap_position.resize(ukey_size, not_in_heap);
    values.resize(ukey_size);
  }

 public:
  DHeapUKeyValueMap(unsigned ukey_size) {
    ResetHeapPosition(ukey_size);
    heap_keys.reserve(ukey_size);
  }

  DHeapUKeyValueMap(const std::vector<TValue>& v, bool skip_heap) : values(v) {
    if (skip_heap) {
      ResetHeapPosition(v.size());
    } else {
      heap_position = heap_keys = Enumerate<unsigned>(0, v.size());
      Heapify();
    }
  }

  bool Empty() const { return heap_keys.empty(); }
  unsigned Size() const { return unsigned(heap_keys.size()); }
  unsigned UKeySize() const { return unsigned(values.size()); }

  const TValue& Get(unsigned key) const { return values[key]; }
  const std::vector<TValue>& GetValues() const { return values; }

 protected:
  void AddNewKeyI(unsigned key, const TValue& new_value, bool skip_heap) {
    values[key] = new_value;
    if (!skip_heap) {
      heap_position[key] = Size();
      heap_keys.push_back(key);
      SiftUp(heap_position[key]);
    }
  }

  void DecreaseValueI(unsigned key, unsigned key_position,
                      const TValue& new_value) {
    values[key] = new_value;
    SiftUp(key_position);
  }

  void IncreaseValueI(unsigned key, unsigned key_position,
                      const TValue& new_value) {
    values[key] = new_value;
    SiftDown(key_position);
  }

 public:
  void AddNewKey(unsigned key, const TValue& new_value,
                 bool skip_heap = false) {
    assert(heap_position[key] == not_in_heap);
    AddNewKeyI(key, new_value, skip_heap);
  }

  void DecreaseValue(unsigned key, const TValue& new_value) {
    unsigned key_position = heap_position[key];
    if (key_position == not_in_heap)
      AddNewKeyI(key, new_value, false);
    else
      DecreaseValueI(key, key_position, new_value);
  }

  void IncreaseValue(unsigned key, const TValue& new_value) {
    unsigned key_position = heap_position[key];
    assert(key_position != not_in_heap);
    IncreaseValueI(key, key_position, new_value);
  }

  void Set(unsigned key, const TValue& new_value) {
    unsigned key_position = heap_position[key];
    if (key_position == not_in_heap)
      AddNewKeyI(key, new_value, false);
    else if (compare(new_value, values[key]))
      DecreaseValueI(key, key_position, new_value);
    else
      IncreaseValueI(key, key_position, new_value);
  }

  void Add(const TData& x) { Set(x.key, x.value); }
  unsigned TopKey() const { return heap_keys[0]; }
  const TValue& TopValue() const { return Get(TopKey()); }

  TData Top() const {
    unsigned key = TopKey();
    return {key, Get(key)};
  }

  void Pop() {
    heap_position[TopKey()] = not_in_heap;
    if (Size() > 1) {
      heap_keys[0] = heap_keys.back();
      heap_keys.pop_back();
      heap_position[TopKey()] = 0;
      SiftDown(0);
    } else {
      heap_keys.pop_back();
    }
  }

  unsigned ExtractKey() {
    unsigned t = TopKey();
    Pop();
    return t;
  }

  TValue ExtractValue() {
    TValue t = TopValue();
    Pop();
    return t;
  }

  TData Extract() {
    TData t = Top();
    Pop();
    return t;
  }

 protected:
  void SiftUp(unsigned pos) {
    if (pos == 0) return;
    unsigned xkey = heap_keys[pos];
    const TValue& xvalue = values[xkey];
    unsigned npos = (pos - 1) / d;
    unsigned nkey = heap_keys[npos];
    if (compare(xvalue, values[nkey])) {
      heap_keys[pos] = nkey;
      heap_position[nkey] = pos;
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / d;
        nkey = heap_keys[npos];
        if (compare(xvalue, values[nkey])) {
          heap_keys[pos] = nkey;
          heap_position[nkey] = pos;
        } else {
          break;
        }
      }
      heap_keys[pos] = xkey;
      heap_position[xkey] = pos;
    }
  }

  void BestChildD(unsigned cb, unsigned& npos, unsigned& nkey) const {
    npos = cb;
    nkey = heap_keys[npos];
    for (unsigned i = cb, j = 1; j < d; ++j) {
      if (Compare(heap_keys[++i], nkey)) {
        npos = i;
        nkey = heap_keys[i];
      }
    }
  }

  bool SiftDownNext(const TValue& xvalue, unsigned pos, unsigned& npos,
                    unsigned& nkey) const {
    unsigned cb = d * pos + 1;
    if (cb >= Size()) return false;
    unsigned ce = cb + d;
    if (ce <= Size()) {
      BestChildD(cb, npos, nkey);
    } else {
      ce = Size();
      npos = cb;
      nkey = heap_keys[npos];
      for (unsigned i = cb + 1; i < ce; ++i) {
        if (Compare(heap_keys[i], nkey)) {
          npos = i;
          nkey = heap_keys[i];
        }
      }
    }
    return compare(values[nkey], xvalue);
  }

  void SiftDown(unsigned pos) {
    unsigned xkey = heap_keys[pos], npos, nkey;
    const TValue& xvalue = values[xkey];
    if (SiftDownNext(xvalue, pos, npos, nkey)) {
      heap_keys[pos] = nkey;
      heap_position[nkey] = pos;
      for (pos = npos;; pos = npos) {
        if (SiftDownNext(xvalue, pos, npos, nkey)) {
          heap_keys[pos] = nkey;
          heap_position[nkey] = pos;
        } else {
          break;
        }
      }
      heap_keys[pos] = xkey;
      heap_position[xkey] = pos;
    }
  }

  void Heapify() {
    for (unsigned pos = Size() / d; pos;) SiftDown(--pos);
  }
};
}  // namespace heap
