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
template <class TValue>
class BucketQueueLL {
 public:
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
  constexpr BucketQueueLL() {}

  constexpr explicit BucketQueueLL(unsigned expected_max_priority) {
    queue.resize(expected_max_priority + 1, nullptr);
  }

  constexpr bool Empty() const { return size == 0; }

  constexpr unsigned Size() const { return size; }

  constexpr void Add(unsigned p, const TValue& value) {
    AdjustQueueSize(p);
    auto node = manager.New();
    node->value = value;
    node->next = queue[p];
    queue[p] = node;
    ++size;
    top_priority = std::min(top_priority, p);
  }

  constexpr unsigned TopPriority() const { return top_priority; }

  constexpr const TValue& TopValue() const {
    assert(!Empty());
    return queue[top_priority]->value;
  }

  constexpr TData Top() const { return {TopPriority(), TopValue()}; }

  constexpr void Pop() {
    assert(!Empty());
    auto node = queue[top_priority];
    queue[top_priority] = node->next;
    manager.Release(node);
    --size;
    if (Empty()) {
      top_priority = -1u;
    } else {
      ShiftPriority();
    }
  }

  constexpr unsigned ExtractPriority() {
    const unsigned t = TopPriority();
    Pop();
    return t;
  }

  constexpr TValue ExtractValue() {
    const TValue v = TopValue();
    Pop();
    return v;
  }

  constexpr TData Extract() {
    const TData t = Top();
    Pop();
    return t;
  }

 protected:
  constexpr void AdjustQueueSize(unsigned p) {
    if (queue.size() <= p) queue.resize(p + 1, nullptr);
  }

  constexpr void ShiftPriority() {
    for (; !queue[top_priority];) ++top_priority;
  }
};
}  // namespace base
}  // namespace heap
