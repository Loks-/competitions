#pragma once

#include "common/base.h"

#include <array>
#include <vector>

namespace heap {
namespace monotone {
namespace base {
// Similar to BucketQueue but use less memory.
// Memory  -- O(N + first_layer_size + P / first_layer_size)
template <unsigned first_layer_size, class TTValue>
class TwoLayersBuckets {
 public:
  static const unsigned fl_size = first_layer_size;
  using TValue = TTValue;
  using TSelf = TwoLayersBuckets<fl_size, TTValue>;

  class TData {
   public:
    unsigned priority;
    TValue value;
  };

 protected:
  std::array<std::vector<TValue>, fl_size> queue1;
  std::vector<std::vector<TData>> queue2;
  unsigned priority = 0, p1b = 0, p1e = fl_size;
  unsigned size = 0;

 public:
  TwoLayersBuckets() {}
  explicit TwoLayersBuckets(unsigned max_priority) {
    queue2.resize(max_priority / fl_size + 1);
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }

  void Add(unsigned p, const TValue& value) {
    AdjustQueueSize(p);
    if (p >= p1e)
      queue2[p / fl_size].push_back({p, value});
    else
      queue1[p - p1b].push_back(value);
    ++size;
  }

  unsigned TopPriority() {
    ShiftPriority();
    return priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue1[priority - p1b].back();
  }

  TData Top() {
    ShiftPriority();
    return {priority, queue1[priority - p1b].back()};
  }

  void Pop() {
    ShiftPriority();
    queue1[priority - p1b].pop_back();
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
    if (queue2.size() * fl_size <= k) queue2.resize(k / fl_size + 1);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (;;) {
      for (; priority < p1e; ++priority) {
        if (!queue1[priority - p1b].empty()) return;
      }
      p1b = p1e;
      p1e += fl_size;
      for (const auto& d : queue2[p1b / fl_size])
        queue1[d.priority - p1b].push_back(d.value);
      queue2[p1b / fl_size].clear();
    }
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
