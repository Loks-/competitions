#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/node.h"
#include "common/nodes_manager_fixed_size.h"

#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// Rolling bucket queue with double linked list
// P - max priority, W - window
// Memory  -- O(N + W)
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(1 + P / N) amortized, O(W) worst case
// Pop     -- O(1 + P / N) amortized, O(W) worst case
// Init    -- O(N)
// top priority <= new priority < top priority + window
class RollingBucketQueueDLL {
 public:
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = RollingBucketQueueDLL;

  class TNode : public BaseNode {
   public:
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  NodesManagerFixedSize<TNode> manager;
  std::vector<unsigned> priorities;
  TNode *pkey0 = nullptr, *ppriority0 = nullptr;
  unsigned top_priority = 0, top_priority_adj = 0;
  unsigned size = 0;
  unsigned window;

 protected:
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }
  TNode* PNode(unsigned priority) { return ppriority0 + priority; }
  unsigned Key(const TNode* node) const { return node - pkey0; }

 public:
  void Reset(unsigned ukey_size, unsigned _window) {
    window = _window;
    manager.Reset(ukey_size + window);
    priorities.resize(ukey_size);
    pkey0 = manager.NodeByRawIndex(0);
    ppriority0 = manager.NodeByRawIndex(ukey_size);
    for (unsigned i = 0; i < window; ++i) {
      auto n = PNode(i);
      n->next = n->prev = n;
    }
    top_priority = top_priority_adj = size = 0;
  }

  RollingBucketQueueDLL(unsigned ukey_size, unsigned _window) {
    Reset(ukey_size, _window);
  }

  RollingBucketQueueDLL(const std::vector<unsigned>& v, bool skip_heap,
                        unsigned _window) {
    Reset(v.size(), _window);
    for (unsigned i = 0; i < v.size(); ++i) priorities[i] = v[i];
    if (!skip_heap) {
      for (unsigned i = 0; i < priorities.size(); ++i) {
        unsigned p = priorities[i];
        assert(p < window);
        auto knode = KNode(i), pnode = PNode(p);
        knode->prev = pnode->prev;
        knode->prev->next = knode;
        pnode->prev = knode;
        knode->next = pnode;
      }
      size = priorities.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(priorities.size()); }

  bool InHeap(unsigned key) const { return KNode(key)->next; }

  unsigned Get(unsigned key) const { return priorities[key]; }

  const std::vector<TValue>& GetValues() const { return priorities; }

 public:
  void AddNewKey(unsigned key, unsigned priority, bool skip_heap = false) {
    assert(!InHeap(key));
    assert(skip_heap ||
           ((top_priority <= priority) && (priority < top_priority + window)));
    AddNewKeyI(key, priority, skip_heap);
  }

  void Set(unsigned key, unsigned new_priority) {
    assert((top_priority <= new_priority) &&
           (new_priority < top_priority + window));
    if (InHeap(key))
      SetI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  void DecreaseValue(unsigned key, unsigned new_priority) {
    Set(key, new_priority);
  }

  void DecreaseValueIfLess(unsigned key, unsigned new_priority) {
    assert((top_priority <= new_priority) &&
           (new_priority < top_priority + window));
    if (new_priority < priorities[key]) Set(key, new_priority);
  }

  void IncreaseValue(unsigned key, unsigned new_priority) {
    Set(key, new_priority);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() {
    ShiftPriority();
    return Key(PNode(top_priority_adj)->next);
  }

  unsigned TopValue() {
    ShiftPriority();
    return top_priority;
  }

  TData Top() { return {TopKey(), TopValue()}; }

  void Pop() {
    ShiftPriority();
    RemoveNodeI(PNode(top_priority_adj)->next);
  }

  unsigned ExtractKey() {
    ShiftPriority();
    auto n = PNode(top_priority_adj)->next;
    RemoveNodeI(n);
    return Key(n);
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

  void DeleteKey(unsigned key) { RemoveNodeI(KNode(key)); }

 protected:
  void ShiftPriority() {
    assert(!Empty());
    for (; PNode(top_priority_adj)->next == PNode(top_priority_adj);
         ++top_priority)
      top_priority_adj = (top_priority_adj + 1) % window;
  }

  void AddNewKeyI(unsigned key, unsigned priority, bool skip_heap) {
    priorities[key] = priority;
    if (!skip_heap) {
      auto knode = KNode(key), pnode = PNode(priority % window);
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
      ++size;
    }
  }

  void SetI(unsigned key, unsigned new_priority) {
    if (priorities[key] != new_priority) {
      auto knode = KNode(key), pnode = PNode(new_priority % window);
      knode->next->prev = knode->prev;
      knode->prev->next = knode->next;
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
    }
  }

  void RemoveNodeI(TNode* node) {
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->prev = node->next = nullptr;
    --size;
  }
};
}  // namespace ukvm
}  // namespace monotone
}  // namespace heap
