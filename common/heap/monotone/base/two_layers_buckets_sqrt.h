#pragma once

#include "common/base.h"
#include "common/numeric/bits/ulog2.h"

#include <vector>

namespace heap {
namespace monotone {
namespace base {
// P - max priority
// Memory  -- O(N + sqrt(P))
// Add     -- O(1)
// Top     -- O(1 + P / N) amortized
// Pop     -- O(1 + P / N) amortized
template <class TTValue>
class TwoLayersBucketsSqrt {
 public:
  using TValue = TTValue;
  using TSelf = TwoLayersBucketsSqrt<TTValue>;

  class TData {
   public:
    unsigned priority;
    TValue value;
  };

 protected:
  std::vector<std::vector<TValue>> queue1;
  std::vector<std::vector<TData>> queue2;
  unsigned top_priority, lq1size, p1b, p1e;
  unsigned size;

 public:
  void Reset(unsigned max_priority) {
    top_priority = 0;
    lq1size = numeric::ULog2(max_priority) / 2 + 1;
    p1b = 0;
    p1e = (1u << lq1size);
    queue1.clear();
    queue1.resize(p1e);
    queue2.clear();
    queue2.resize(max_priority / p1e + 1);
    size = 0;
  }

  TwoLayersBucketsSqrt() { Reset(16); }
  explicit TwoLayersBucketsSqrt(unsigned max_priority) { Reset(max_priority); }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }

  void Add(unsigned p, const TValue& value) {
    AdjustQueueSize(p);
    if (p >= p1e)
      queue2[p >> lq1size].push_back({p, value});
    else
      queue1[p - p1b].push_back(value);
    ++size;
  }

  unsigned TopPriority() {
    ShiftPriority();
    return top_priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue1[top_priority - p1b].back();
  }

  TData Top() {
    ShiftPriority();
    return {top_priority, queue1[top_priority - p1b].back()};
  }

  void Pop() {
    ShiftPriority();
    queue1[top_priority - p1b].pop_back();
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
    p >>= lq1size;
    if (queue2.size() <= p) queue2.resize(p + 1);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; top_priority < p1e; ++top_priority) {
      if (!queue1[top_priority - p1b].empty()) return;
    }
    for (p1b = p1e; queue2[p1b >> lq1size].empty();) p1b += queue1.size();
    p1e = p1b + queue1.size();
    for (const auto& d : queue2[p1b >> lq1size])
      queue1[d.priority - p1b].push_back(d.value);
    queue2[p1b >> lq1size].clear();
    for (top_priority = p1b;; ++top_priority) {
      if (!queue1[top_priority - p1b].empty()) return;
    }
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
