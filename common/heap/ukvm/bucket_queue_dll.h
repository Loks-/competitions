#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <algorithm>
#include <vector>

namespace heap {
namespace ukvm {
// Bucket queue with double linked list
// P - max priority
// Memory  -- O(N + P)
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(1)
// Pop     -- O(1 + P / N) amortized if monotone, O(P) otherwise
class BucketQueueDLL {
 public:
  using TValue = unsigned;
  using TData = Data<TValue>;
  using TSelf = BucketQueueDLL;

  class TNode : public memory::Node {
   public:
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  std::vector<TNode> nodes_key;
  memory::NodesManager<TNode> manager_priority;
  std::vector<unsigned> priority;
  std::vector<TNode*> queue;
  TNode* pkey0;
  unsigned top_priority;
  unsigned size;

 protected:
  constexpr TNode* KNode(unsigned key) { return pkey0 + key; }

  constexpr const TNode* KNode(unsigned key) const { return pkey0 + key; }

  constexpr TNode* PNode(unsigned p) { return queue[p]; }

  constexpr const TNode* PNode(unsigned p) const { return queue[p]; }

  constexpr unsigned Key(const TNode* node) const { return node - pkey0; }

 public:
  constexpr void Reset(unsigned ukey_size) {
    nodes_key.clear();
    nodes_key.resize(ukey_size);
    manager_priority.clear();
    priority.clear();
    priority.resize(ukey_size, -1u);
    queue.clear();
    pkey0 = &(nodes_key[0]);
    top_priority = -1u;
    size = 0;
  }

  constexpr explicit BucketQueueDLL(unsigned ukey_size) { Reset(ukey_size); }

  constexpr BucketQueueDLL(const std::vector<unsigned>& v, bool skip_heap)
      : priority(v) {
    Reset(v.size());
    priority = v;
    if (!skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        AdjustQueueSize(p);
        auto knode = KNode(i), pnode = PNode(p);
        knode->prev = pnode->prev;
        knode->prev->next = knode;
        pnode->prev = knode;
        knode->next = pnode;
      }
      size = v.size();
      ResetPriority();
    }
  }

  constexpr bool Empty() const { return size == 0; }

  constexpr unsigned Size() const { return size; }

  constexpr unsigned UKeySize() const { return unsigned(priority.size()); }

  constexpr bool InHeap(unsigned key) const { return KNode(key)->next; }

  constexpr unsigned Get(unsigned key) const { return priority[key]; }

  constexpr const std::vector<TValue>& GetValues() const { return priority; }

 public:
  constexpr void AddNewKey(unsigned key, unsigned _priority,
                           bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, _priority, skip_heap);
  }

  constexpr void Set(unsigned key, unsigned new_priority) {
    if (InHeap(key))
      SetI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  constexpr void DecreaseValue(unsigned key, unsigned new_priority) {
    Set(key, new_priority);
  }

  constexpr void DecreaseValueIfLess(unsigned key, unsigned new_priority) {
    if (new_priority < priority[key]) Set(key, new_priority);
  }

  constexpr void IncreaseValue(unsigned key, unsigned new_priority) {
    Set(key, new_priority);
  }

  constexpr void Add(const TData& x) { Set(x.key, x.value); }

  constexpr unsigned TopKey() const { return Key(TopNode()); }

  constexpr unsigned TopValue() const { return top_priority; }

  constexpr TData Top() const { return {TopKey(), TopValue()}; }

  constexpr void Pop() { RemoveNodeI(TopNode()); }

  constexpr unsigned ExtractKey() {
    auto node = TopNode();
    RemoveNodeI(node);
    return Key(node);
  }

  constexpr unsigned ExtractValue() {
    const unsigned t = TopValue();
    Pop();
    return t;
  }

  constexpr TData Extract() {
    const TData t = Top();
    Pop();
    return t;
  }

  constexpr void DeleteKey(unsigned key) { RemoveNodeI(KNode(key)); }

 protected:
  constexpr void AdjustQueueSize(unsigned p) {
    unsigned s = queue.size();
    if (s <= p) {
      queue.resize(p + 1);
      for (; s <= p; ++s) {
        auto n = manager_priority.create();
        n->next = n->prev = n;
        queue[s] = n;
      }
    }
  }

  constexpr void ShiftPriority() {
    for (; queue[top_priority]->next == queue[top_priority];) ++top_priority;
  }

  constexpr void ResetPriority() {
    if (Empty()) {
      top_priority = -1u;
    } else {
      top_priority = 0;
      ShiftPriority();
    }
  }

  constexpr TNode* TopNode() { return PNode(top_priority)->next; }

  constexpr const TNode* TopNode() const { return PNode(top_priority)->next; }

  constexpr void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      AdjustQueueSize(p);
      auto knode = KNode(key), pnode = PNode(p);
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
      ++size;
      top_priority = std::min(top_priority, p);
    }
  }

  constexpr void SetI(unsigned key, unsigned new_priority) {
    if (priority[key] != new_priority) {
      auto knode = KNode(key), pnode = PNode(new_priority);
      knode->next->prev = knode->prev;
      knode->prev->next = knode->next;
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
      if (new_priority <= top_priority) {
        top_priority = new_priority;
      } else if (priority[key] == top_priority) {
        ShiftPriority();
      }
      priority[key] = new_priority;
    }
  }

  constexpr void RemoveNodeI(TNode* node) {
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->prev = node->next = nullptr;
    --size;
    if (Empty())
      top_priority = -1u;
    else
      ShiftPriority();
  }
};
}  // namespace ukvm
}  // namespace heap
