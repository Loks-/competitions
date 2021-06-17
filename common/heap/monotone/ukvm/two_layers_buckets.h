#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

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
  using TSelf = BucketQueue;

  struct Position {
    unsigned priority = not_in_queue;
    unsigned index = not_in_queue;
  };

 protected:
  std::vector<Position> queue_position;
  std::array<std::vector<unsigned>, fl_size> queue1;
  std::vector<std::vector<unsigned>> queue2;
  unsigned top_priority = 0, p1b = 0, p1e = fl_size;
  unsigned size = 0;

 protected:
  void ResetHeapPosition(unsigned ukey_size) {
    queue_position.clear();
    queue_position.resize(ukey_size);
  }

  std::vector<unsigned>& GetQ(unsigned priority) {
    return (priority < p1e) ? queue1[priority - p1b]
                            : queue2[priority / fl_size];
  }

 public:
  explicit TwoLayersBuckets(unsigned ukey_size) {
    ResetHeapPosition(ukey_size);
  }

  TwoLayersBuckets(const std::vector<unsigned>& v, bool skip_heap) {
    ResetHeapPosition(v.size());
    if (skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) queue_position[i].priority = v[i];
    } else {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        AdjustQueueSize(p);
        queue_position[i].priority = p;
        auto& q = GetQ(p);
        queue_position[i].index = q.size();
        q.push_back(i);
      }
      size = v.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(queue_position.size()); }

  bool InHeap(unsigned key) const {
    return queue_position[key].index != not_in_queue;
  }

  unsigned Get(unsigned key) const { return queue_position[key].priority; }

  std::vector<TValue> GetValues() const {
    unsigned n = UKeySize();
    std::vector<TValue> v(n);
    for (unsigned i = 0; i < n; ++i) v[i] = queue_position[i].priority;
    return v;
  }

 public:
  void AddNewKey(unsigned key, unsigned priority, bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, priority, skip_heap);
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
    if (new_priority < queue_position[key].priority) Set(key, new_priority);
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
    DeleteI(queue_position[key]);
    queue_position[key].index = not_in_queue;
  }

 protected:
  void AdjustQueueSize(unsigned k) {
    if (queue2.size() * fl_size <= k) queue2.resize(k / fl_size + 1);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (;;) {
      for (; top_priority < p1e; ++top_priority) {
        if (!queue1[top_priority - p1b].empty()) return;
      }
      p1b = p1e;
      p1e += fl_size;
      for (const auto& k : queue2[p1b / fl_size]) {
        auto& qp = queue_position[k];
        unsigned p = qp.priority;
        qp.index = queue1[p - p1b].size();
        queue1[p - p1b].push_back(k);
      }
      queue2[p1b / fl_size].clear();
    }
  }

  void AddNewKeyI(unsigned key, unsigned priority, bool skip_heap) {
    queue_position[key].priority = priority;
    if (!skip_heap) {
      AdjustQueueSize(priority);
      auto& q = GetQ(priority);
      queue_position[key].index = q.size();
      q.push_back(key);
      ++size;
    }
  }

  void SetI(unsigned key, unsigned new_priority) {
    Position& qp = queue_position[key];
    bool update_required =
        (new_priority < p1e)
            ? (new_priority != qp.priority)
            : (new_priority / fl_size != qp.priority / fl_size);
    if (update_required) {
      AddNewKeyI(key, new_priority, false);
      DeleteI(qp);
    } else {
      qp.priority = new_priority;
    }
  }

  void DeleteI(const Position& pos) {
    assert(pos.index != not_in_queue);
    auto& q = GetQ(pos.priority);
    if (pos.index == q.size() - 1) {
      q.pop_back();
    } else {
      q[pos.index] = q.back();
      queue_position[q.back()].index = pos.index;
      q.pop_back();
    }
    --size;
  }
};
}  // namespace ukvm
}  // namespace monotone
}  // namespace heap
