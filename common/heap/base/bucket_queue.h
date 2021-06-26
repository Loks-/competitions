#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace heap {
namespace base {
// P - max priority
// Memory  -- O(N + P)
// Add     -- O(1)
// Top     -- O(1)
// Pop     -- O(1 + P / N) amortized if monotone, O(P) otherwise
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
  unsigned top_priority = -1u;
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
    top_priority = std::min(top_priority, p);
  }

  unsigned TopPriority() const { return top_priority; }

  const TValue& TopValue() const {
    assert(!Empty());
    return queue[top_priority].back();
  }

  TData Top() const { return {TopPriority(), TopValue()}; }

  void Pop() {
    assert(!Empty());
    queue[top_priority].pop_back();
    --size;
    if (Empty())
      top_priority = -1u;
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
  void AdjustQueueSize(unsigned p) {
    if (queue.size() <= p) queue.resize(p + 1);
  }

  void ShiftPriority() {
    for (; queue[top_priority].size() == 0;) ++top_priority;
  }
};
}  // namespace base
}  // namespace heap
