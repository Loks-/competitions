#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

#include <algorithm>
#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// P - max priority, W - window
// Memory  -- O(N + log W)
// Add     -- O(log W)
// DecV    -- O(1) amortized
// IncV    -- O(log W)
// Top     -- O(1) amortized
// Pop     -- O(1) amortized
class RadixW {
 public:
  static const unsigned not_in_queue = -1u;
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = RadixW;

 protected:
  std::vector<unsigned> priority;
  std::vector<unsigned> index;
  std::vector<unsigned> position;
  std::vector<std::vector<unsigned>> queue;
  std::vector<unsigned> vfirst, vlength;
  unsigned current_index;
  unsigned size;

 protected:
  unsigned Index(unsigned p, unsigned l) {
    for (; vfirst[l] > p;) --l;
    return l;
  }

  unsigned Index(unsigned p) { return Index(p, queue.size() - 1); }

 public:
  void Reset(unsigned ukey_size, unsigned window) {
    priority.clear();
    priority.resize(ukey_size, -1u);
    index.clear();
    index.resize(ukey_size, -1u);
    position.clear();
    position.resize(ukey_size, -1u);
    vfirst.clear();
    vfirst.push_back(0);
    vfirst.push_back(1);
    vlength.resize(2, 1);
    window = std::max(window, 2u);
    for (; vlength.back() < window;) {
      vfirst.push_back(vfirst.back() + vlength.back());
      vlength.push_back(vlength.back() * 2);
    }
    queue.clear();
    queue.resize(vlength.size());
    current_index = 0;
    size = 0;
  }

  RadixW(unsigned ukey_size, unsigned window) { Reset(ukey_size, window); }

  RadixW(const std::vector<unsigned>& v, bool skip_heap, unsigned window) {
    Reset(v.size(), window);
    priority = v;
    if (!skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        auto ind = Index(p);
        index[i] = ind;
        position[i] = queue[ind].size();
        queue[ind].push_back(i);
      }
      size = v.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(priority.size()); }
  bool InHeap(unsigned key) const { return index[key] != not_in_queue; }
  unsigned Get(unsigned key) const { return priority[key]; }
  const std::vector<TValue>& GetValues() const { return priority; }

 public:
  void AddNewKey(unsigned key, unsigned _priority, bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, _priority, skip_heap);
  }

  void Set(unsigned key, unsigned new_priority) {
    if (!InHeap(key))
      AddNewKeyI(key, new_priority, false);
    else if (priority[key] < new_priority)
      IncreaseI(key, new_priority);
    else
      DecreaseI(key, new_priority);
  }

  void DecreaseValue(unsigned key, unsigned new_priority) {
    if (InHeap(key))
      DecreaseI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  void DecreaseValueIfLess(unsigned key, unsigned new_priority) {
    if (new_priority < priority[key]) DecreaseValue(key, new_priority);
  }

  void IncreaseValue(unsigned key, unsigned new_priority) {
    if (InHeap(key))
      IncreaseI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() {
    ShiftPriority();
    return queue[current_index].back();
  }

  unsigned TopValue() { return priority[TopKey()]; }

  TData Top() {
    auto key = TopKey();
    return {key, priority[key]};
  }

  void Pop() { DeleteKey(TopKey()); }

  unsigned ExtractKey() {
    unsigned key = TopKey();
    DeleteKey(key);
    return key;
  }

  unsigned ExtractValue() {
    unsigned key = ExtractKey();
    return priority[key];
  }

  TData Extract() {
    unsigned key = ExtractKey();
    return {key, priority[key]};
  }

  void DeleteKey(unsigned key) {
    RemoveI(key);
    index[key] = not_in_queue;
    position[key] = not_in_queue;
    --size;
  }

 protected:
  void ShiftPriority() {
    assert(!Empty());
    for (; queue[current_index].size() == 0;) ++current_index;
    if ((current_index < 2) || ((queue[current_index].size() == 1) && (current_index + 1 < queue.size()))) return;
    auto& q = queue[current_index];
    unsigned minp = priority[q[0]];
    for (unsigned i = 1; i < q.size(); ++i)
      minp = std::min(minp, priority[q[i]]);
    vfirst[0] = minp;
    for (unsigned i = 0; i < current_index; ++i)
      vfirst[i + 1] = vfirst[i] + vlength[i];
    for (auto key : q) {
      auto new_index = Index(priority[key], current_index - 1);
      index[key] = new_index;
      position[key] = queue[new_index].size();
      queue[new_index].push_back(key);
    }
    q.clear();
    current_index = 0;
    assert(queue[0].size() > 0);
  }

  void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      auto ind = Index(p);
      index[key] = ind;
      position[key] = queue[ind].size();
      queue[ind].push_back(key);
      ++size;
    }
  }

  void MoveI(unsigned key, unsigned new_index) {
    RemoveI(key);
    index[key] = new_index;
    position[key] = queue[new_index].size();
    queue[new_index].push_back(key);
  }

  void DecreaseI(unsigned key, unsigned new_priority) {
    priority[key] = new_priority;
    unsigned new_index = Index(new_priority, index[key]);
    if (new_index != index[key]) MoveI(key, new_index);
  }

  void IncreaseI(unsigned key, unsigned new_priority) {
    priority[key] = new_priority;
    unsigned new_index = Index(new_priority);
    if (new_index != index[key]) MoveI(key, new_index);
  }

  void RemoveI(unsigned key) {
    unsigned ind = index[key];
    assert(ind != not_in_queue);
    unsigned pos = position[key];
    auto& q = queue[ind];
    if (pos < q.size() - 1) {
      q[pos] = q.back();
      position[q.back()] = pos;
    }
    q.pop_back();
  }
};
}  // namespace ukvm
}  // namespace monotone
}  // namespace heap
