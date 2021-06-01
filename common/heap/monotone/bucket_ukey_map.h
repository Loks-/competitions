#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace heap {
namespace monotone {
// P - max priority, W - window
// Memory  -- O(N + W)
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(1) amortized
// Pop     -- O(1 + P / N) amortized, O(W) worst case
// Init    -- O(N)
// top priority <= new priority < top priority + window
class BucketUKeyMap {
 public:
  static const unsigned not_in_queue = -1u;
  using TValue = unsigned;
  using TSelf = BucketUKeyMap;

  struct TData {
    unsigned priority;
    unsigned key;
  };

  struct Position {
    unsigned priority = not_in_queue;
    unsigned index = not_in_queue;
  };

 protected:
  std::vector<Position> queue_position;
  std::vector<std::vector<unsigned>> queue;
  unsigned top_priority = 0, top_priority_adj = 0;
  unsigned size = 0;
  unsigned window;

 protected:
  void ResetHeapPosition(unsigned ukey_size) {
    queue_position.clear();
    queue_position.resize(ukey_size);
  }

 public:
  void SetWindow(unsigned _window) {
    window = _window;
    queue.resize(window);
  }

  BucketUKeyMap(unsigned ukey_size, unsigned _window) {
    ResetHeapPosition(ukey_size);
    SetWindow(_window);
  }

  BucketUKeyMap(const std::vector<unsigned>& v, bool skip_heap,
                unsigned _window) {
    ResetHeapPosition(v.size());
    SetWindow(_window);
    if (skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) queue_position[i].priority = v[i];
    } else {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        assert(p < window);
        queue_position[i].priority = p;
        queue_position[i].index = queue[p].size();
        queue[p].push_back(i);
      }
      size = v.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(queue_position.size()); }

  bool HasKey(unsigned key) const {
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
    assert(queue_position[key].index == not_in_queue);
    assert(skip_heap ||
           ((top_priority <= priority) && (priority < top_priority + window)));
    AddNewKeyI(key, priority, skip_heap);
  }

  void SetPriority(unsigned key, unsigned new_priority) {
    assert((top_priority <= new_priority) &&
           (new_priority < top_priority + window));
    if (queue_position[key].index == not_in_queue)
      AddNewKeyI(key, new_priority, false);
    else
      SetPriorityI(key, new_priority);
  }

  void DecreasePriorityIfLess(unsigned key, unsigned new_priority) {
    assert((top_priority <= new_priority) &&
           (new_priority < top_priority + window));
    if (new_priority < queue_position[key].priority)
      SetPriority(key, new_priority);
  }

  void DecreaseValueIfLess(unsigned key, unsigned new_value) {
    DecreasePriorityIfLess(key, new_value);
  }

  void Add(const TData& x) { SetPriority(x.key, x.priority); }

  unsigned TopPriority() {
    ShiftPriority();
    return top_priority;
  }

  unsigned TopKey() {
    ShiftPriority();
    return queue[top_priority_adj].back();
  }

  TData Top() { return {TopPriority(), TopKey()}; }

  void Pop() { DeleteKey(TopKey()); }

  unsigned ExtractKey() {
    unsigned t = TopKey();
    Pop();
    return t;
  }

  unsigned ExtractPriority() {
    unsigned t = TopPriority();
    Pop();
    return t;
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
  void ShiftPriority() {
    assert(!Empty());
    for (; queue[top_priority_adj].size() == 0; ++top_priority)
      top_priority_adj = (top_priority_adj + 1) % window;
  }

  void AddNewKeyI(unsigned key, unsigned priority, bool skip_heap) {
    queue_position[key].priority = priority;
    if (!skip_heap) {
      queue_position[key].index = queue[priority % window].size();
      queue[priority % window].push_back(key);
      ++size;
    }
  }

  void SetPriorityI(unsigned key, unsigned new_priority) {
    Position old = queue_position[key];
    if (old.priority != new_priority) {
      AddNewKeyI(key, new_priority, false);
      DeleteI(old);
    }
  }

  void DeleteI(const Position& pos) {
    assert(pos.index != not_in_queue);
    auto priority_adj = pos.priority % window;
    if (pos.index == queue[priority_adj].size() - 1) {
      queue[priority_adj].pop_back();
    } else {
      queue[priority_adj][pos.index] = queue[priority_adj].back();
      queue_position[queue[priority_adj].back()].index = pos.index;
      queue[priority_adj].pop_back();
    }
    --size;
  }
};
}  // namespace monotone
}  // namespace heap
