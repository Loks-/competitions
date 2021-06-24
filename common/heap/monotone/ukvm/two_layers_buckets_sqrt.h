#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// P - max priority
// Memory  -- O(N + sqrt(P))
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(1 + P / N) amortized
// Pop     -- O(1 + P / N) amortizedtemplate <unsigned first_layer_size>
class TwoLayersBucketsSqrt {
 public:
  static const unsigned not_in_queue = -1u;
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = TwoLayersBucketsSqrt;

 protected:
  std::vector<unsigned> priority;
  std::vector<unsigned> position;
  std::vector<std::vector<unsigned>> queue1, queue2;
  unsigned top_priority, lq1size, p1b, p1e;
  unsigned size;

 protected:
  std::vector<unsigned>& GetQ(unsigned priority) {
    return (priority < p1e) ? queue1[priority - p1b]
                            : queue2[priority >> lq1size];
  }

 public:
  void Reset(unsigned ukey_size, unsigned max_priority) {
    priority.clear();
    priority.resize(ukey_size, -1u);
    position.clear();
    position.resize(ukey_size, -1u);
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

  TwoLayersBucketsSqrt(unsigned ukey_size, unsigned max_priority) {
    Reset(ukey_size, max_priority);
  }

  TwoLayersBucketsSqrt(const std::vector<unsigned>& v, bool skip_heap,
                       unsigned max_priority) {
    Reset(v.size(), max_priority);
    priority = v;
    if (!skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        AdjustQueueSize(p);
        auto& q = GetQ(p);
        position[i] = q.size();
        q.push_back(i);
      }
      size = v.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(priority.size()); }
  bool InHeap(unsigned key) const { return position[key] != not_in_queue; }
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
    return queue1[top_priority - p1b].back();
  }

  unsigned TopValue() {
    ShiftPriority();
    return top_priority;
  }

  TData Top() {
    ShiftPriority();
    return {queue1[top_priority - p1b].back(), top_priority};
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
    for (const auto& key : queue2[p1b >> lq1size]) {
      unsigned p = priority[key];
      position[key] = queue1[p - p1b].size();
      queue1[p - p1b].push_back(key);
    }
    queue2[p1b >> lq1size].clear();
    for (top_priority = p1b;; ++top_priority) {
      if (!queue1[top_priority - p1b].empty()) return;
    }
  }

  void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      AdjustQueueSize(p);
      auto& q = GetQ(p);
      position[key] = q.size();
      q.push_back(key);
      ++size;
    }
  }

  void SetI(unsigned key, unsigned new_priority) {
    auto p = priority[key];
    bool update_required = (p < p1e)
                               ? (new_priority != p)
                               : ((new_priority >> lq1size) != (p >> lq1size));
    if (update_required) {
      DeleteI(key);
      AddNewKeyI(key, new_priority, false);
    } else {
      priority[key] = new_priority;
    }
  }

  void DeleteI(unsigned key) {
    unsigned pos = position[key];
    assert(pos != not_in_queue);
    auto& q = GetQ(priority[key]);
    if (pos < q.size() - 1) {
      q[pos] = q.back();
      position[q.back()] = pos;
    }
    q.pop_back();
    --size;
  }
};
}  // namespace ukvm
}  // namespace monotone
}  // namespace heap
