#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

#include <functional>
#include <vector>

namespace heap {
namespace ext {
// Binary Heap with map to position by key.
// Values are stored inside heap.
// Memory  -- O(N)
// Add     -- O(log N / log d)
// DecV    -- O(log N / log d)
// IncV    -- O(d log N / log d)
// Top     -- O(1)
// Pop     -- O(d log N / log d)
// Init    -- O(N)
template <unsigned d, class TTValue, class TTCompare = std::less<TTValue>>
class DHeapUKeyPosMap {
 public:
  static constexpr unsigned not_in_heap = unsigned(-1);

  using TValue = TTValue;
  using TData = heap::ukvm::Data<TValue>;
  using TCompare = TTCompare;
  using TSelf = DHeapUKeyPosMap<d, TValue, TCompare>;

 protected:
  TCompare compare;
  std::vector<TData> data;
  std::vector<unsigned> heap_position;

 protected:
  constexpr bool Compare(const TData& l, const TData& r) const {
    return compare(l.value, r.value);
  }

  constexpr void ResetHeapPosition(unsigned ukey_size) {
    heap_position.clear();
    heap_position.resize(ukey_size, not_in_heap);
  }

 public:
  constexpr explicit DHeapUKeyPosMap(unsigned ukey_size) {
    ResetHeapPosition(ukey_size);
    data.reserve(ukey_size);
  }

  constexpr DHeapUKeyPosMap(unsigned ukey_size, const std::vector<TData>& v)
      : data(v) {
    ResetHeapPosition(ukey_size);
    Heapify();
  }

  constexpr bool Empty() const { return data.empty(); }

  constexpr unsigned Size() const { return unsigned(data.size()); }

  constexpr unsigned UKeySize() const { return unsigned(heap_position.size()); }

  constexpr TValue Get(unsigned key) const {
    const unsigned key_position = heap_position[key];
    assert(key_position != not_in_heap);
    return data[key_position];
  }

 protected:
  constexpr void AddNewKeyI(unsigned key, const TValue& new_value) {
    heap_position[key] = Size();
    data.push_back({key, new_value});
    SiftUp(heap_position[key]);
  }

  constexpr void DecreaseValueI(unsigned key_position,
                                const TValue& new_value) {
    data[key_position].value = new_value;
    SiftUp(key_position);
  }

  constexpr void IncreaseValueI(unsigned key_position,
                                const TValue& new_value) {
    data[key_position].value = new_value;
    SiftDown(key_position);
  }

 public:
  constexpr void AddNewKey(unsigned key, const TValue& new_value) {
    assert(heap_position[key] == not_in_heap);
    AddNewKeyI(key, new_value);
  }

  constexpr void DecreaseValue(unsigned key, const TValue& new_value) {
    const unsigned key_position = heap_position[key];
    if (key_position == not_in_heap)
      AddNewKeyI(key, new_value);
    else
      DecreaseValueI(key_position, new_value);
  }

  constexpr void DecreaseValueIfLess(unsigned key, const TValue& new_value) {
    const unsigned key_position = heap_position[key];
    if (key_position == not_in_heap)
      AddNewKeyI(key, new_value);
    else if (compare(new_value, data[key_position].value))
      DecreaseValueI(key_position, new_value);
  }

  constexpr void IncreaseValue(unsigned key, const TValue& new_value) {
    const unsigned key_position = heap_position[key];
    assert(key_position != not_in_heap);
    IncreaseValueI(key_position, new_value);
  }

  constexpr void Set(unsigned key, const TValue& new_value) {
    const unsigned key_position = heap_position[key];
    if (key_position == not_in_heap)
      AddNewKeyI(key, new_value);
    else if (compare(new_value, data[key_position].value))
      DecreaseValueI(key_position, new_value);
    else
      IncreaseValueI(key_position, new_value);
  }

  constexpr void Add(const TData& x) { Set(x.key, x.value); }

  constexpr const TData& Top() const { return data[0]; }

  constexpr unsigned TopKey() const { return data[0].key; }

  constexpr const TValue& TopValue() const { return data[0].value; }

  constexpr void Pop() {
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

  constexpr TData Extract() {
    const TData t = Top();
    Pop();
    return t;
  }

  constexpr unsigned ExtractKey() {
    const unsigned t = TopKey();
    Pop();
    return t;
  }

  constexpr TValue ExtractValue() {
    const TValue t = TopValue();
    Pop();
    return t;
  }

 protected:
  constexpr void SiftUp(unsigned pos) {
    if (pos == 0) return;
    unsigned npos = (pos - 1) / d;
    if (Compare(data[pos], data[npos])) {
      TData x = data[pos];
      data[pos] = data[npos];
      heap_position[data[pos].key] = pos;
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / d;
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

  constexpr bool SiftDownNext(const TData& x, unsigned pos,
                              unsigned& npos) const {
    const unsigned cb = d * pos + 1, ce = std::min(cb + d, Size());
    if (cb >= ce) return false;
    npos = cb;
    for (unsigned i = cb + 1; i < ce; ++i) {
      if (Compare(data[i], data[npos])) npos = i;
    }
    return compare(data[npos].value, x.value);
  }

  constexpr void SiftDown(unsigned pos) {
    unsigned npos;
    TData x = data[pos];
    if (SiftDownNext(x, pos, npos)) {
      data[pos] = data[npos];
      heap_position[data[pos].key] = pos;
      for (pos = npos;; pos = npos) {
        if (SiftDownNext(x, pos, npos)) {
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

  constexpr void Heapify() {
    for (unsigned pos = 0; pos < Size(); ++pos)
      heap_position[data[pos].key] = pos;
    for (unsigned pos = Size() / d; pos;) SiftDown(--pos);
  }
};
}  // namespace ext
}  // namespace heap
