#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"

#include <vector>

namespace heap {
namespace monotone {
namespace base {
// Rolling Bucket queue with linked list to reduce memory allocations
// P - max priority, W - window
// Memory  -- O(N + W)
// Add     -- O(1)
// Top     -- O(1 + P / N) amortized, O(W) worst case
// Pop     -- O(1 + P / N) amortized, O(W) worst case
// current priority <= new priority < current priority + window
template <class TTValue>
class RollingBucketQueueLL {
 public:
  using TValue = TTValue;
  using TSelf = RollingBucketQueueLL<TTValue>;

  class TData {
   public:
    unsigned priority;
    TValue value;
  };

  class TNode : public BaseNode {
   public:
    TValue value;
    TNode* next = nullptr;
  };

 protected:
  NodesManager<TNode> manager;
  std::vector<TNode*> queue;
  unsigned priority = 0, priority_adj = 0;
  unsigned size = 0;
  unsigned window;

 public:
  RollingBucketQueueLL() {}
  explicit RollingBucketQueueLL(unsigned _window) { SetWindow(_window); }

  void SetWindow(unsigned _window) {
    window = _window;
    queue.resize(window, nullptr);
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }

  void Add(unsigned p, const TValue& value) {
    assert((priority <= p) && (p < priority + window));
    auto node = manager.New();
    node->value = value;
    node->next = queue[p % window];
    queue[p % window] = node;
    ++size;
  }

  unsigned TopPriority() {
    ShiftPriority();
    return priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue[priority_adj]->value;
  }

  TData Top() {
    ShiftPriority();
    return {priority, queue[priority_adj]->value};
  }

  void Pop() {
    ShiftPriority();
    auto node = queue[priority_adj];
    queue[priority_adj] = node->next;
    manager.Release(node);
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
  void ShiftPriority() {
    assert(!Empty());
    for (; !queue[priority_adj]; ++priority)
      priority_adj = (priority_adj + 1) % window;
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
