#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace heap {
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
  unsigned top_priority = not_in_queue;
  unsigned size = 0;

 protected:
  void ResetHeapPosition(unsigned ukey_size) {
    queue_position.clear();
    queue_position.resize(ukey_size);
  }

 public:
  explicit BucketUKeyMap(unsigned ukey_size) { ResetHeapPosition(ukey_size); }

  BucketUKeyMap(const std::vector<unsigned>& v, bool skip_heap) {
    ResetHeapPosition(v.size());
    if (skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) queue_position[i].priority = v[i];
    } else {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        AdjustQueueSize(p);
        queue_position[i].priority = p;
        queue_position[i].index = queue[p].size();
        queue[p].push_back(i);
      }
      size = v.size();
      ResetPriority();
    }
  }

  bool Empty() const { return Size() == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(queue_position.size()); }

  bool HasKey(unsigned key) const {
    return queue_position[key].index != not_in_queue;
  }

  unsigned GetPriority(unsigned key) const {
    return queue_position[key].priority;
  }

 public:
  void AddNewKey(unsigned key, unsigned priority, bool skip_heap = false) {
    assert(queue_position[key].index == not_in_queue);
    AddNewKeyI(key, priority, skip_heap);
  }

  void SetPriority(unsigned key, unsigned new_priority) {
    if (queue_position[key].index == not_in_queue)
      AddNewKeyI(key, new_priority, false);
    else
      SetPriorityI(key, new_priority);
  }

  void DecreasePriorityIfLess(unsigned key, unsigned new_priority) {
    if (new_priority < queue_position[key].priority)
      SetPriority(key, new_priority);
  }

  void Add(const TData& x) { SetPriority(x.key, x.priority); }
  unsigned TopPriority() const { return top_priority; }
  unsigned TopKey() const { return queue[top_priority].back(); }

  TData Top() const { return {TopPriority(), TopKey()}; }

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
  void AdjustQueueSize(unsigned k) {
    if (queue.size() <= k) queue.resize(k + 1);
  }

  void ShiftPriority() {
    for (; queue[top_priority].size() == 0;) ++top_priority;
  }

  void ResetPriority() {
    if (Empty()) {
      top_priority = -1u;
    } else {
      top_priority = 0;
      ShiftPriority();
    }
  }

  void AddNewKeyI(unsigned key, unsigned priority, bool skip_heap) {
    queue_position[key].priority = priority;
    if (!skip_heap) {
      AdjustQueueSize(priority);
      queue_position[key].index = queue[priority].size();
      queue[priority].push_back(key);
      ++size;
      top_priority = std::min(top_priority, priority);
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
    if (pos.index == queue[pos.priority].size() - 1) {
      queue[pos.priority].pop_back();
    } else {
      queue[pos.priority][pos.index] = queue[pos.priority].back();
      queue_position[queue[pos.priority].back()].index = pos.index;
      queue[pos.priority].pop_back();
    }
    --size;
    ShiftPriority();
  }
};
}  // namespace heap
