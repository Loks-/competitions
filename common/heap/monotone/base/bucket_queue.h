#pragma once

#include "common/base.h"

#include <vector>

namespace heap {
namespace monotone {
namespace base {
// P - max priority
// Memory  -- O(N + P)
// Add     -- O(1)
// Top     -- O(1 + P / N) amortized
// Pop     -- O(1 + P / N) amortized
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
  unsigned priority = 0;
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
  }

  unsigned TopPriority() {
    ShiftPriority();
    return priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue[priority].back();
  }

  TData Top() {
    ShiftPriority();
    return {priority, queue[priority].back()};
  }

  void Pop() {
    ShiftPriority();
    queue[priority].pop_back();
    --size;
  }

  unsigned ExtractPriority() {
    Pop();
    return priority;
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
    assert(!Empty());
    for (; queue[priority].size() == 0;) ++priority;
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
