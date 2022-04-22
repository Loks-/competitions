#pragma once

#include "common/base.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <algorithm>
#include <vector>

namespace heap {
namespace base {
// Bucket queue with linked list to reduce memory allocations
// P - max priority
// Memory  -- O(N + P)
// Add     -- O(1)
// Top     -- O(1)
// Pop     -- O(1 + P / N) amortized if monotone, O(P) otherwise
template <class TTValue>
class BucketQueueLL {
 public:
  using TValue = TTValue;
  using TSelf = BucketQueueLL<TTValue>;

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
  unsigned top_priority = -1u;
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
    top_priority = std::min(top_priority, p);
  }

  unsigned TopPriority() const { return top_priority; }

  const TValue& TopValue() const {
    assert(!Empty());
    return queue[top_priority]->value;
  }

  TData Top() const { return {TopPriority(), TopValue()}; }

  void Pop() {
    assert(!Empty());
    auto node = queue[top_priority];
    queue[top_priority] = node->next;
    manager.Release(node);
    --size;
    if (Empty())
      top_priority = -1u;
    else
      ShiftPriority();
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
  void AdjustQueueSize(unsigned p) {
    if (queue.size() <= p) queue.resize(p + 1, nullptr);
  }

  void ShiftPriority() {
    for (; !queue[top_priority];) ++top_priority;
  }
};
}  // namespace base
}  // namespace heap
