#pragma once

#include "common/base.h"

#include <stack>
#include <vector>

namespace heap {
namespace monotone {
namespace base {
// P - max priority, W - window
// Memory  -- O(N + W)
// Add     -- O(1)
// Top     -- O(1 + P / N) amortized, O(W) worst case
// Pop     -- O(1 + P / N) amortized, O(W) worst case
// current priority <= new priority < current priority + window
template <class TTValue>
class RollingBucketQueue {
 public:
  using TValue = TTValue;
  using TSelf = RollingBucketQueue<TTValue>;

  class TData {
   public:
    unsigned priority;
    TValue value;
  };

 protected:
  std::vector<std::stack<TValue>> queue;
  unsigned top_priority = 0, top_priority_adj = 0;
  unsigned size = 0;
  unsigned window;

 public:
  RollingBucketQueue() {}
  explicit RollingBucketQueue(unsigned _window) { SetWindow(_window); }

  void SetWindow(unsigned _window) {
    window = _window;
    queue.resize(window);
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }

  void Add(unsigned p, const TValue& value) {
    queue[p % window].push(value);
    ++size;
  }

  unsigned TopPriority() {
    ShiftPriority();
    return top_priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue[top_priority_adj].top();
  }

  TData Top() {
    ShiftPriority();
    return {top_priority, queue[top_priority_adj].top()};
  }

  void Pop() {
    ShiftPriority();
    queue[top_priority_adj].pop();
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
  void ShiftPriority() {
    assert(!Empty());
    for (; queue[top_priority_adj].size() == 0; ++top_priority)
      top_priority_adj = (top_priority_adj + 1) % window;
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
