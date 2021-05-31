#pragma once

#include <algorithm>
#include <vector>

namespace heap {
template <class TTValue>
class BucketQueue {
 public:
  using TValue = TTValue;
  using TSelf = BucketQueue<TTValue>;

  class TData {
   public:
    unsigned priority;
    TValue value;
  };

 protected:
  std::vector<std::vector<TValue>> queue;
  unsigned priority = -1u;
  unsigned size = 0;

 public:
  BucketQueue() {}
  explicit BucketQueue(unsigned max_priority) {
    queue.resize(max_priority + 1);
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }

  void Add(unsigned p, const TValue& value) {
    AdjustQueueSize(p);
    queue[p].push_back(value);
    ++size;
    priority = std::min(priority, p);
  }

  unsigned TopPriority() const { return priority; }

  const TValue& TopValue() const {
    assert(!Empty());
    return queue[priority].back();
  }

  TData Top() const { return {TopPriority(), TopValue()}; }

  void Pop() {
    assert(!Empty());
    queue[priority].pop_back();
    --size;
    if (Empty())
      priority = -1u;
    else
      ShiftPriority();
  }

  unsigned ExtractPriority() {
    unsigned t = TopPriority();
    Pop();
    return t;
  }

  TValue ExtractValue() {
    TValue v = TopValue();
    Pop();
    return v;
  }

  TData Extract() {
    TData t = Top();
    Pop();
    return t;
  }

 protected:
  void AdjustQueueSize(unsigned k) {
    if (queue.size() <= k) queue.resize(k + 1);
  }

  void ShiftPriority() {
    for (; queue[priority].size() == 0;) ++priority;
  }
};
}  // namespace heap
