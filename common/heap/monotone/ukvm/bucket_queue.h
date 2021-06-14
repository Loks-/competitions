#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// P - max priority
// Memory  -- O(N + P)
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(1 + P / N) amortized
// Pop     -- O(1 + P / N) amortized
class BucketQueue {
 public:
  static const unsigned not_in_queue = -1u;
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = BucketQueue;

  struct Position {
    unsigned priority = not_in_queue;
    unsigned index = not_in_queue;
  };

 protected:
  std::vector<Position> queue_position;
  std::vector<std::vector<unsigned>> queue;
  unsigned top_priority = 0;
  unsigned size = 0;

 protected:
  void ResetHeapPosition(unsigned ukey_size) {
    queue_position.clear();
    queue_position.resize(ukey_size);
  }

 public:
  explicit BucketQueue(unsigned ukey_size) { ResetHeapPosition(ukey_size); }

  BucketQueue(const std::vector<unsigned>& v, bool skip_heap) {
    ResetHeapPosition(v.size());
    if (skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) queue_position[i].priority = v[i];
    } else {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        AdjustQueueSize(p);
        queue_position[i].priority = p;
        queue_position[i].index = queue[p].size();
        queue[p].push_back(i);
      }
      size = v.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(queue_position.size()); }

  bool InHeap(unsigned key) const {
    return queue_position[key].index != not_in_queue;
  }

  unsigned Get(unsigned key) const { return queue_position[key].priority; }

  std::vector<TValue> GetValues() const {
    unsigned n = UKeySize();
    std::vector<TValue> v(n);
    for (unsigned i = 0; i < n; ++i) v[i] = queue_position[i].priority;
    return v;
  }

 public:
  void AddNewKey(unsigned key, unsigned priority, bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, priority, skip_heap);
  }

  void Set(unsigned key, unsigned new_priority) {
    if (InHeap(key))
      SetI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  void DecreaseValue(unsigned key, unsigned new_priority) {
    Set(key, new_priority);
  }

  void DecreaseValueIfLess(unsigned key, unsigned new_priority) {
    if (new_priority < queue_position[key].priority) Set(key, new_priority);
  }

  void IncreaseValue(unsigned key, unsigned new_priority) {
    Set(key, new_priority);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() {
    ShiftPriority();
    return queue[top_priority].back();
  }

  unsigned TopValue() {
    ShiftPriority();
    return top_priority;
  }

  TData Top() {
    ShiftPriority();
    return {queue[top_priority].back(), top_priority};
  }

  void Pop() { DeleteKey(TopKey()); }

  unsigned ExtractKey() {
    unsigned key = TopKey();
    DeleteKey(key);
    return key;
  }

  unsigned ExtractValue() {
    Pop();
    return top_priority;
  }

  TData Extract() {
    TData t = Top();
    Pop();
    return t;
  }

  void DeleteKey(unsigned key) {
    DeleteI(queue_position[key]);
    queue_position[key].index = not_in_queue;
  }

 protected:
  void AdjustQueueSize(unsigned k) {
    if (queue.size() <= k) queue.resize(k + 1);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; queue[top_priority].size() == 0;) ++top_priority;
  }

  void AddNewKeyI(unsigned key, unsigned priority, bool skip_heap) {
    queue_position[key].priority = priority;
    if (!skip_heap) {
      AdjustQueueSize(priority);
      queue_position[key].index = queue[priority].size();
      queue[priority].push_back(key);
      ++size;
    }
  }

  void SetI(unsigned key, unsigned new_priority) {
    Position old = queue_position[key];
    if (old.priority != new_priority) {
      AddNewKeyI(key, new_priority, false);
      DeleteI(old);
    }
  }

  void DeleteI(const Position& pos) {
    assert(pos.index != not_in_queue);
    if (pos.index == queue[pos.priority].size() - 1) {
      queue[pos.priority].pop_back();
    } else {
      queue[pos.priority][pos.index] = queue[pos.priority].back();
      queue_position[queue[pos.priority].back()].index = pos.index;
      queue[pos.priority].pop_back();
    }
    --size;
  }
};
}  // namespace ukvm
}  // namespace monotone
}  // namespace heap
