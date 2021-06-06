#pragma once

#include "common/base.h"

#include <algorithm>
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
  unsigned priority = 0, priority_adj = 0;
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
    assert((priority <= p) && (p < priority + window));
    queue[p % window].push(value);
    ++size;
  }

  unsigned TopPriority() {
    ShiftPriority();
    return priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue[priority_adj].top();
  }

  TData Top() { return {TopPriority(), TopValue()}; }

  void Pop() {
    ShiftPriority();
    queue[priority_adj].pop();
    --size;
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
  void ShiftPriority() {
    assert(!Empty());
    for (; queue[priority_adj].size() == 0; ++priority)
      priority_adj = (priority_adj + 1) % window;
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
