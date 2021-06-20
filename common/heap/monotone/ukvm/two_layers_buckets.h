#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

#include <algorithm>
#include <array>
#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// Similar to BucketQueue but use less memory.
// Memory  -- O(N + first_layer_size + P / first_layer_size)
template <unsigned first_layer_size>
class TwoLayersBuckets {
 public:
  static const unsigned not_in_queue = -1u;
  static const unsigned fl_size = first_layer_size;
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = TwoLayersBuckets<fl_size>;

 protected:
  std::vector<unsigned> priority;
  std::vector<unsigned> position;
  std::array<std::vector<unsigned>, fl_size> queue1;
  std::vector<std::vector<unsigned>> queue2;
  unsigned top_priority, p1b, p1e;
  unsigned size;

 protected:
  std::vector<unsigned>& GetQ(unsigned priority) {
    return (priority < p1e) ? queue1[priority - p1b]
                            : queue2[priority / fl_size];
  }

 public:
  void Reset(unsigned ukey_size) {
    priority.clear();
    priority.resize(ukey_size, -1u);
    position.clear();
    position.resize(ukey_size, -1u);
    std::fill(queue1.begin(), queue1.end(), std::vector<unsigned>());
    queue2.clear();
    top_priority = p1b = 0;
    p1e = fl_size;
    size = 0;
  }

  explicit TwoLayersBuckets(unsigned ukey_size) { Reset(ukey_size); }

  TwoLayersBuckets(const std::vector<unsigned>& v, bool skip_heap) {
    Reset(v.size());
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
    if (queue2.size() * fl_size <= p) queue2.resize(p / fl_size + 1);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; top_priority < p1e; ++top_priority) {
      if (!queue1[top_priority - p1b].empty()) return;
    }
    for (p1b = p1e; queue2[p1b / fl_size].empty();) p1b += fl_size;
    p1e = p1b + fl_size;
    for (const auto& key : queue2[p1b / fl_size]) {
      unsigned p = priority[key];
      position[key] = queue1[p - p1b].size();
      queue1[p - p1b].push_back(key);
    }
    queue2[p1b / fl_size].clear();
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
    bool update_required = (p < p1e) ? (new_priority != p)
                                     : (new_priority / fl_size != p / fl_size);
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
