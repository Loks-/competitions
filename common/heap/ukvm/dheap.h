#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace heap {
namespace ukvm {
// Memory  -- O(N)
// Add     -- O(log N / log d)
// DecV    -- O(log N / log d)
// IncV    -- O(d log N / log d)
// Top     -- O(1)
// Pop     -- O(d log N / log d)
// Init    -- O(N)
template <unsigned d_, class TTValue, class TTCompare = std::less<TTValue>>
class DHeap {
 public:
  const unsigned d = d_;
  const unsigned not_in_heap = unsigned(-1);
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TData = Data<TValue>;
  using TSelf = DHeap<d_, TValue, TCompare>;

  struct TPositionValue {
    unsigned heap_position;
    TValue value;
  };

  using TPointer = TPositionValue*;

 protected:
  TCompare compare;
  std::vector<TPointer> heap_pointers;
  std::vector<TPositionValue> key_map;
  TPointer pbegin;

 protected:
  bool Compare(TPointer pl, TPointer pr) const {
    return compare(pl->value, pr->value);
  }

 public:
  explicit DHeap(unsigned ukey_size) {
    key_map.resize(ukey_size, {not_in_heap, TValue()});
    pbegin = &(key_map[0]);
    heap_pointers.reserve(ukey_size);
  }

  DHeap(const std::vector<TValue>& v, bool skip_heap) : DHeap(v.size()) {
    unsigned n = UKeySize();
    if (skip_heap) {
      for (unsigned i = 0; i < n; ++i) key_map[i].value = v[i];
    } else {
      for (unsigned i = 0; i < n; ++i) {
        key_map[i].value = v[i];
        key_map[i].heap_position = i;
        heap_pointers.push_back(pbegin + i);
      }
      Heapify();
    }
  }

  bool Empty() const { return heap_pointers.empty(); }
  unsigned Size() const { return unsigned(heap_pointers.size()); }
  unsigned UKeySize() const { return unsigned(key_map.size()); }

  bool InHeap(unsigned key) const {
    return key_map[key].heap_position != not_in_heap;
  }

  const TValue& Get(unsigned key) const { return key_map[key].value; }

  std::vector<TValue> GetValues() const {
    unsigned n = UKeySize();
    std::vector<TValue> v(n);
    for (unsigned i = 0; i < n; ++i) v[i] = key_map[i].value;
    return v;
  }

 protected:
  void AddNewKeyI(unsigned key, const TValue& new_value, bool skip_heap) {
    key_map[key].value = new_value;
    if (!skip_heap) {
      key_map[key].heap_position = Size();
      heap_pointers.push_back(pbegin + key);
      SiftUp(key_map[key].heap_position);
    }
  }

  void DecreaseValueI(unsigned key, unsigned key_position,
                      const TValue& new_value) {
    key_map[key].value = new_value;
    SiftUp(key_position);
  }

  void IncreaseValueI(unsigned key, unsigned key_position,
                      const TValue& new_value) {
    key_map[key].value = new_value;
    SiftDown(key_position);
  }

 public:
  void AddNewKey(unsigned key, const TValue& new_value,
                 bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, new_value, skip_heap);
  }

  void DecreaseValue(unsigned key, const TValue& new_value) {
    unsigned key_position = key_map[key].heap_position;
    if (key_position == not_in_heap)
      AddNewKeyI(key, new_value, false);
    else
      DecreaseValueI(key, key_position, new_value);
  }

  void DecreaseValueIfLess(unsigned key, const TValue& new_value) {
    if (compare(new_value, key_map[key].value)) DecreaseValue(key, new_value);
  }

  void IncreaseValue(unsigned key, const TValue& new_value) {
    unsigned key_position = key_map[key].heap_position;
    assert(key_position != not_in_heap);
    IncreaseValueI(key, key_position, new_value);
  }

  void Set(unsigned key, const TValue& new_value) {
    unsigned key_position = key_map[key].heap_position;
    if (key_position == not_in_heap)
      AddNewKeyI(key, new_value, false);
    else if (compare(new_value, key_map[key].value))
      DecreaseValueI(key, key_position, new_value);
    else
      IncreaseValueI(key, key_position, new_value);
  }

  void Add(const TData& x) { Set(x.key, x.value); }
  unsigned TopKey() const { return heap_pointers[0] - pbegin; }
  const TValue& TopValue() const { return heap_pointers[0]->value; }

  TData Top() const {
    TPointer p = heap_pointers[0];
    return {p - pbegin, p->value};
  }

  void Pop() {
    TPointer p = heap_pointers[0];
    p->heap_position = not_in_heap;
    if (Size() > 1) {
      TPointer pt = heap_pointers.back();
      heap_pointers[0] = pt;
      pt->heap_position = 0;
      heap_pointers.pop_back();
      SiftDown(0);
    } else {
      heap_pointers.pop_back();
    }
  }

  unsigned ExtractKey() {
    unsigned t = TopKey();
    Pop();
    return t;
  }

  const TValue& ExtractValue() {
    const TValue& t = TopValue();
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
    TPointer p = heap_pointers[pos];
    unsigned npos = (pos - 1) / d;
    TPointer pnext = heap_pointers[npos];
    if (Compare(p, pnext)) {
      heap_pointers[pos] = pnext;
      pnext->heap_position = pos;
      for (pos = npos; pos; pos = npos) {
        npos = (pos - 1) / d;
        pnext = heap_pointers[npos];
        if (Compare(p, pnext)) {
          heap_pointers[pos] = pnext;
          pnext->heap_position = pos;
        } else {
          break;
        }
      }
      heap_pointers[pos] = p;
      p->heap_position = pos;
    }
  }

  bool SiftDownNext(TPointer p, unsigned pos, unsigned& npos,
                    TPointer& pnext) const {
    unsigned cb = d * pos + 1, ce = std::min(cb + d, Size());
    if (cb >= ce) return false;
    npos = cb;
    pnext = heap_pointers[npos];
    for (unsigned i = cb + 1; i < ce; ++i) {
      if (Compare(heap_pointers[i], pnext)) {
        npos = i;
        pnext = heap_pointers[i];
      }
    }
    return Compare(pnext, p);
  }

  void SiftDown(unsigned pos) {
    TPointer p = heap_pointers[pos], pnext;
    unsigned npos;
    if (SiftDownNext(p, pos, npos, pnext)) {
      heap_pointers[pos] = pnext;
      pnext->heap_position = pos;
      for (pos = npos;; pos = npos) {
        if (SiftDownNext(p, pos, npos, pnext)) {
          heap_pointers[pos] = pnext;
          pnext->heap_position = pos;
        } else {
          break;
        }
      }
      heap_pointers[pos] = p;
      p->heap_position = pos;
    }
  }

  void Heapify() {
    for (unsigned pos = Size() / d; pos;) SiftDown(--pos);
  }
};
}  // namespace ukvm
}  // namespace heap
