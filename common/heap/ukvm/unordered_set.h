#pragma once

#include "common/base.h"
#include "common/data_structures/unsigned_set.h"
#include "common/heap/ukvm/data.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace heap {
namespace ukvm {
// Memory  -- O(N)
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(N)
// Pop     -- O(N)
// Init    -- O(N)
template <class TTValue, class TTCompare = std::less<TTValue>>
class UnorderedSet {
 public:
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TData = Data<TValue>;
  using TSelf = UnorderedSet<TValue, TCompare>;

 protected:
  TCompare compare;
  ds::UnsignedSet queue;
  std::vector<TValue> values;

 protected:
  bool Compare(unsigned l, unsigned r) const {
    return compare(values[l], values[r]);
  }

 public:
  explicit UnorderedSet(unsigned ukey_size)
      : queue(ukey_size), values(ukey_size) {}

  UnorderedSet(const std::vector<TValue>& v, bool skip_heap)
      : queue(v.size()), values(v) {
    if (!skip_heap) queue.InsertAll();
  }

  bool Empty() const { return queue.Empty(); }
  unsigned Size() const { return queue.Size(); }
  unsigned UKeySize() const { return queue.SetSize(); }

  bool InHeap(unsigned key) const { return queue.HasKey(key); }

  const TValue& Get(unsigned key) const { return values[key]; }

  const std::vector<TValue>& GetValues() const { return values; }

 protected:
  void AddNewKeyI(unsigned key, const TValue& new_value, bool skip_heap) {
    if (!skip_heap) queue.Insert(key);
    SetI(key, new_value);
  }

  void SetI(unsigned key, const TValue& new_value) { values[key] = new_value; }

 public:
  void AddNewKey(unsigned key, const TValue& new_value,
                 bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, new_value, skip_heap);
  }

  void Set(unsigned key, const TValue& new_value) {
    if (InHeap(key))
      SetI(key, new_value);
    else
      AddNewKeyI(key, new_value, false);
  }
  void DecreaseValue(unsigned key, const TValue& new_value) {
    Set(key, new_value);
  }

  void DecreaseValueIfLess(unsigned key, const TValue& new_value) {
    if (compare(new_value, values[key])) Set(key, new_value);
  }

  void IncreaseValue(unsigned key, const TValue& new_value) {
    Set(key, new_value);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() const {
    assert(!Empty());
    auto& v = queue.List();
    unsigned t = v[0];
    for (unsigned i = 1; i < v.size(); ++i) {
      if (Compare(v[i], t)) t = v[i];
    }
    return t;
  }

  const TValue& TopValue() const { Get(TopKey()); }

  TData Top() const {
    unsigned key = TopKey();
    return {key, Get(key)};
  }

  void DeleteKey(unsigned key) { queue.Remove(key); }

  void Pop() { DeleteKey(TopKey()); }

  unsigned ExtractKey() {
    unsigned key = TopKey();
    DeleteKey(key);
    return key;
  }

  const TValue& ExtractValue() {
    unsigned key = TopKey();
    DeleteKey(key);
    return Get(key);
  }

  TData Extract() {
    unsigned key = TopKey();
    DeleteKey(key);
    return {key, Get(key)};
  }
};
}  // namespace ukvm
}  // namespace heap
