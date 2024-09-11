#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace heap {
namespace ukvm {
// Memory  -- O(N)
// Add     -- O(log N)
// DecV    -- O(log N)
// IncV    -- O(log N)
// Top     -- O(1)
// Pop     -- O(log N)
// Init    -- O(N)
template <class TTValue, class TTCompare = std::less<TTValue>>
class CompleteBinaryTree {
 public:
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TData = Data<TValue>;
  using TSelf = CompleteBinaryTree<TValue, TCompare>;

 protected:
  unsigned size, cbt_size;
  TCompare compare;
  std::vector<TValue> values;
  std::vector<unsigned> in_heap;
  std::vector<unsigned> heap;

 protected:
  constexpr unsigned CBTSize(unsigned n) const {
    unsigned s = 1;
    for (; s < n;) s *= 2;
    return s;
  }

  constexpr bool Compare(unsigned i, unsigned j) const {
    return (in_heap[i] && in_heap[j]) ? compare(values[i], values[j])
                                      : in_heap[i] >= in_heap[j];
  }

  constexpr void InitHeap() {
    cbt_size = CBTSize(values.size());
    in_heap.resize(cbt_size, 0);
    heap.resize(2 * cbt_size);
    heap[0] = unsigned(-1);
    for (unsigned i = 0; i < cbt_size; ++i) heap[i + cbt_size] = i;
  }

 public:
  constexpr explicit CompleteBinaryTree(unsigned ukey_size)
      : size(0), values(ukey_size) {
    InitHeap();
    HeapifyEmpty();
  }

  constexpr CompleteBinaryTree(const std::vector<TValue>& v, bool skip_heap)
      : values(v) {
    size = skip_heap ? 0 : v.size();
    InitHeap();
    if (!skip_heap) {
      std::fill(in_heap.begin(), in_heap.begin() + size, 1u);
      Heapify();
    } else {
      HeapifyEmpty();
    }
  }

  constexpr bool Empty() const { return size == 0; }

  constexpr unsigned Size() const { return size; }

  constexpr unsigned UKeySize() const { return unsigned(values.size()); }

  constexpr bool InHeap(unsigned key) const { return in_heap[key]; }

  constexpr const TValue& Get(unsigned key) const { return values[key]; }

  constexpr const std::vector<TValue>& GetValues() const { return values; }

 protected:
  constexpr void AddNewKeyI(unsigned key, const TValue& new_value,
                            bool skip_heap) {
    values[key] = new_value;
    if (!skip_heap) {
      ++size;
      in_heap[key] = true;
      SiftUp(key);
    }
  }

  constexpr void DecreaseValueI(unsigned key, const TValue& new_value) {
    values[key] = new_value;
    SiftUp(key);
  }

  constexpr void IncreaseValueI(unsigned key, const TValue& new_value) {
    values[key] = new_value;
    SiftDown(key);
  }

 public:
  constexpr void AddNewKey(unsigned key, const TValue& new_value,
                           bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, new_value, skip_heap);
  }

  constexpr void DecreaseValue(unsigned key, const TValue& new_value) {
    if (InHeap(key))
      DecreaseValueI(key, new_value);
    else
      AddNewKeyI(key, new_value, false);
  }

  constexpr void DecreaseValueIfLess(unsigned key, const TValue& new_value) {
    if (compare(new_value, Get(key))) DecreaseValue(key, new_value);
  }

  constexpr void IncreaseValue(unsigned key, const TValue& new_value) {
    if (InHeap(key))
      IncreaseValueI(key, new_value);
    else
      AddNewKeyI(key, new_value, false);
  }

  constexpr void Set(unsigned key, const TValue& new_value) {
    if (!InHeap(key))
      AddNewKeyI(key, new_value, false);
    else if (compare(new_value, Get(key)))
      DecreaseValueI(key, new_value);
    else
      IncreaseValueI(key, new_value);
  }

  constexpr void Add(const TData& x) { Set(x.key, x.value); }

  constexpr unsigned TopKey() const { return heap[1]; }

  constexpr const TValue& TopValue() const { return Get(TopKey()); }

  constexpr TData Top() const {
    const unsigned key = TopKey();
    return {key, Get(key)};
  }

  constexpr void DeleteKey(unsigned key) {
    if (in_heap[key]) {
      --size;
      in_heap[key] = false;
      for (unsigned p = cbt_size + key, pnext;; p = pnext) {
        pnext = p / 2;
        if (heap[pnext] != key) break;
        heap[pnext] = Compare(heap[p], heap[p ^ 1]) ? heap[p] : heap[p ^ 1];
      }
    }
  }

  constexpr void Pop() { DeleteKey(TopKey()); }

  constexpr unsigned ExtractKey() {
    const unsigned t = TopKey();
    Pop();
    return t;
  }

  constexpr const TValue& ExtractValue() {
    const TValue& t = TopValue();
    Pop();
    return t;
  }

  constexpr TData Extract() {
    const TData t = Top();
    Pop();
    return t;
  }

 protected:
  constexpr void SiftDown(unsigned key) {
    unsigned p = cbt_size + key, pnext;
    for (;; p = pnext) {
      pnext = p / 2;
      if (heap[pnext] != key) break;
      if (!Compare(key, heap[p ^ 1])) {
        heap[pnext] = heap[p ^ 1];
        break;
      }
    }
    for (p = pnext;; p = pnext) {
      pnext = p / 2;
      if (heap[pnext] != key) break;
      heap[pnext] = Compare(heap[p], heap[p ^ 1]) ? heap[p] : heap[p ^ 1];
    }
  }

  constexpr void SiftUp(unsigned key) {
    unsigned p = (cbt_size + key) / 2;
    for (; heap[p] == key;) p /= 2;
    for (; p; p /= 2) {
      if (Compare(key, heap[p]))
        heap[p] = key;
      else
        break;
    }
  }

  constexpr void HeapifyEmpty() {
    for (unsigned i = cbt_size - 1; i > 0; --i) heap[i] = heap[2 * i];
  }

  constexpr void Heapify() {
    for (unsigned i = cbt_size - 1; i > 0; --i)
      heap[i] =
          Compare(heap[2 * i], heap[2 * i + 1]) ? heap[2 * i] : heap[2 * i + 1];
  }
};
}  // namespace ukvm
}  // namespace heap
