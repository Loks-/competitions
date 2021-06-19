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

 protected:
  std::vector<unsigned> priority;
  std::vector<unsigned> position;
  std::vector<std::vector<unsigned>> queue;
  unsigned top_priority;
  unsigned size;

 public:
  void Reset(unsigned ukey_size) {
    priority.clear();
    priority.resize(ukey_size, -1u);
    position.clear();
    position.resize(ukey_size, -1u);
    queue.clear();
    top_priority = 0;
    size = 0;
  }

  explicit BucketQueue(unsigned ukey_size) { Reset(ukey_size); }

  BucketQueue(const std::vector<unsigned>& v, bool skip_heap) {
    Reset(v.size());
    priority = v;
    if (!skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        AdjustQueueSize(p);
        position[i] = queue[p].size();
        queue[p].push_back(i);
      }
      size = v.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(priority.size()); }

  bool InHeap(unsigned key) const {
    return position[key] != not_in_queue;
  }

  unsigned Get(unsigned key) const { return priority[key]; }

  const std::vector<TValue>& GetValues() const { return priority; }

 public:
  void AddNewKey(unsigned key, unsigned _priority, bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, _priority, skip_heap);
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
    if (new_priority < priority[key]) Set(key, new_priority);
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
    DeleteI(key);
    position[key] = not_in_queue;
  }

 protected:
  void AdjustQueueSize(unsigned p) {
    if (queue.size() <= p) queue.resize(p + 1);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; queue[top_priority].size() == 0;) ++top_priority;
  }

  void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      AdjustQueueSize(p);
      position[key] = queue[p].size();
      queue[p].push_back(key);
      ++size;
    }
  }

  void SetI(unsigned key, unsigned new_priority) {
    if (priority[key] != new_priority) {
      DeleteI(key);
      AddNewKeyI(key, new_priority, false);
    }
  }

  void DeleteI(unsigned key) {
    unsigned pos = position[key];
    assert(pos != not_in_queue);
    auto& qp = queue[priority[key]];
    if (pos < qp.size() - 1) {
      qp[pos] = qp.back();
      position[qp.back()] = pos;
    }
    qp.pop_back();
    --size;
  }
};
}  // namespace ukvm
}  // namespace monotone
}  // namespace heap
