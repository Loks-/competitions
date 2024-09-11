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
template <class TValue>
class BucketQueue {
 public:
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
  constexpr BucketQueue() {}

  constexpr explicit BucketQueue(unsigned expected_max_priority) {
    queue.resize(expected_max_priority + 1);
  }

  constexpr bool Empty() const { return size == 0; }

  constexpr unsigned Size() const { return size; }

  constexpr void Add(unsigned p, const TValue& value) {
    AdjustQueueSize(p);
    queue[p].push_back(value);
    ++size;
    top_priority = std::min(top_priority, p);
  }

  constexpr unsigned TopPriority() const { return top_priority; }

  constexpr const TValue& TopValue() const {
    assert(!Empty());
    return queue[top_priority].back();
  }

  constexpr TData Top() const { return {TopPriority(), TopValue()}; }

  constexpr void Pop() {
    assert(!Empty());
    queue[top_priority].pop_back();
    --size;
    if (Empty()) {
      top_priority = -1u;
    } else {
      ShiftPriority();
    }
  }

  constexpr unsigned ExtractPriority() {
    const unsigned t = TopPriority();
    Pop();
    return t;
  }

  constexpr TValue ExtractValue() {
    const TValue v = TopValue();
    Pop();
    return v;
  }

  constexpr TData Extract() {
    const TData t = Top();
    Pop();
    return t;
  }

 protected:
  constexpr void AdjustQueueSize(unsigned p) {
    if (queue.size() <= p) queue.resize(p + 1);
  }

  constexpr void ShiftPriority() {
    for (; queue[top_priority].size() == 0;) ++top_priority;
  }
};
}  // namespace base
}  // namespace heap
