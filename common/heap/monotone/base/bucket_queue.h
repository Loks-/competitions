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
  unsigned top_priority = 0;
  unsigned size = 0;

 public:
  BucketQueue() {}

  explicit BucketQueue(unsigned expected_max_priority) {
    queue.resize(expected_max_priority + 1);
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
    return top_priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue[top_priority].back();
  }

  TData Top() {
    ShiftPriority();
    return {top_priority, queue[top_priority].back()};
  }

  void Pop() {
    ShiftPriority();
    queue[top_priority].pop_back();
    --size;
  }

  unsigned ExtractPriority() {
    Pop();
    return top_priority;
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
  void AdjustQueueSize(unsigned p) {
    if (queue.size() <= p) queue.resize(p + 1);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; queue[top_priority].size() == 0;) ++top_priority;
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
