#pragma once

#include "common/base.h"
#include "common/vector/enumerate.h"
#include <functional>
#include <vector>

namespace heap {
template <class TTValue, class TTCompare = std::less<TTValue>>
class UKeyValueMap {
 public:
  const unsigned not_in_heap = unsigned(-1);
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TSelf = UKeyValueMap<TValue, TCompare>;

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
  bool Compare(unsigned lkey, unsigned rkey) {
    return compare(values[lkey], values[rkey]);
  }

  void ResetHeapPosition(unsigned ukey_size) {
    heap_position.clear();
    heap_position.resize(ukey_size, not_in_heap);
    values.resize(ukey_size);
  }

 public:
  UKeyValueMap(unsigned ukey_size) {
    ResetHeapPosition(ukey_size);
    heap_keys.reserve(ukey_size);
  }

  UKeyValueMap(const std::vector<TValue>& v, bool skip_heap) : values(v) {
    if (skip_heap) {
      ResetHeapPosition(v.size());
    } else {
      heap_position = heap_keys = Enumerate<unsigned>(0, v.size());
      Heapify();
    }
  }

  bool Empty() const { return heap_keys.empty(); }
  size_t Size() const { return heap_keys.size(); }
  unsigned UKeySize() const { return unsigned(values.size()); }

  const TValue& Get(unsigned key) const { return values[key]; }
  const std::vector<TValue>& GetValues() const { return values; }

 protected:
  void AddNewKeyI(unsigned key, const TValue& new_value, bool skip_heap) {
    values[key] = new_value;
    if (!skip_heap) {
      heap_position[key] = heap_keys.size();
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
      AddNewKeyI(key, new_value);
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
    unsigned npos = (pos - 1) / 2;
    if (Compare(heap_keys[pos], heap_keys[npos])) {
      unsigned xkey = heap_keys[pos];
      heap_keys[pos] = heap_keys[npos];
      heap_position[heap_keys[pos]] = pos;
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / 2;
        if (Compare(xkey, heap_keys[npos])) {
          heap_keys[pos] = heap_keys[npos];
          heap_position[heap_keys[pos]] = pos;
        } else {
          break;
        }
      }
      heap_keys[pos] = xkey;
      heap_position[xkey] = pos;
    }
  }

  void SiftDown(unsigned pos) {
    unsigned npos = 2 * pos + 1;
    if (npos >= heap_keys.size()) return;
    if ((npos + 1 < heap_keys.size()) &&
        Compare(heap_keys[npos + 1], heap_keys[npos]))
      ++npos;
    if (Compare(heap_keys[npos], heap_keys[pos])) {
      unsigned xkey = heap_keys[pos];
      heap_keys[pos] = heap_keys[npos];
      heap_position[heap_keys[pos]] = pos;
      for (pos = npos;; pos = npos) {
        npos = 2 * pos + 1;
        if (npos >= heap_keys.size()) break;
        if ((npos + 1 < heap_keys.size()) &&
            Compare(heap_keys[npos + 1], heap_keys[npos]))
          ++npos;
        if (Compare(heap_keys[npos], xkey)) {
          heap_keys[pos] = heap_keys[npos];
          heap_position[heap_keys[pos]] = pos;
        } else {
          break;
        }
      }
      heap_keys[pos] = xkey;
      heap_position[xkey] = pos;
    }
  }

 public:
  void Heapify() {
    for (unsigned pos = heap_keys.size() / 2; pos;) SiftDown(--pos);
  }
};
}  // namespace heap
