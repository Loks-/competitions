#pragma once

#include "common/base.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <vector>

namespace heap {
namespace monotone {
namespace base {
// Bucket queue with linked list to reduce memory allocations
// P - max priority
// Memory  -- O(N + P)
// Add     -- O(1)
// Top     -- O(1 + P / N) amortized
// Pop     -- O(1 + P / N) amortized
template <class TTValue>
class BucketQueueLL {
 public:
  using TValue = TTValue;
  using TSelf = BucketQueue<TTValue>;

  class TData {
   public:
    unsigned priority;
    TValue value;
  };

  class TNode : public memory::Node {
   public:
    TValue value;
    TNode* next = nullptr;
  };

 protected:
  memory::NodesManager<TNode> manager;
  std::vector<TNode*> queue;
  unsigned top_priority = 0;
  unsigned size = 0;

 public:
  BucketQueueLL() {}

  explicit BucketQueueLL(unsigned expected_max_priority) {
    queue.resize(expected_max_priority + 1, nullptr);
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }

  void Add(unsigned p, const TValue& value) {
    AdjustQueueSize(p);
    auto node = manager.New();
    node->value = value;
    node->next = queue[p];
    queue[p] = node;
    ++size;
  }

  unsigned TopPriority() {
    ShiftPriority();
    return top_priority;
  }

  const TValue& TopValue() {
    ShiftPriority();
    return queue[top_priority]->value;
  }

  TData Top() {
    ShiftPriority();
    return {top_priority, queue[top_priority]->value};
  }

  void Pop() {
    ShiftPriority();
    auto node = queue[top_priority];
    queue[top_priority] = node->next;
    manager.Release(node);
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
    if (queue.size() <= p) queue.resize(p + 1, nullptr);
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; !queue[top_priority];) ++top_priority;
  }
};
}  // namespace base
}  // namespace monotone
}  // namespace heap
