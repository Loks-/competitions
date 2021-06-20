#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

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

  class TNode {
   public:
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  std::vector<TNode> nodes;
  std::vector<unsigned> priority;
  TNode *pkey0, *ppriority0;
  unsigned top_priority, top_priority_adj;
  unsigned size;
  unsigned window;

 protected:
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }
  TNode* PNodeAdj(unsigned p_adj) { return ppriority0 + p_adj; }
  TNode* PNode(unsigned p) { return ppriority0 + (p % window); }
  unsigned Key(const TNode* node) const { return node - pkey0; }

 public:
  void Reset(unsigned ukey_size, unsigned _window) {
    window = _window;
    nodes.clear();
    nodes.resize(ukey_size + window);
    priority.clear();
    priority.resize(ukey_size, -1u);
    pkey0 = &(nodes[0]);
    ppriority0 = pkey0 + ukey_size;
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
    priority = v;
    if (!skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        assert(p < window);
        auto knode = KNode(i), pnode = PNodeAdj(p);
        knode->prev = pnode->prev;
        knode->prev->next = knode;
        pnode->prev = knode;
        knode->next = pnode;
      }
      size = v.size();
    }
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(priority.size()); }
  bool InHeap(unsigned key) const { return KNode(key)->next; }
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
    return Key(TopNode());
  }

  unsigned TopValue() {
    ShiftPriority();
    return top_priority;
  }

  TData Top() {
    ShiftPriority();
    return {Key(TopNode()), top_priority};
  }

  void Pop() {
    ShiftPriority();
    RemoveNodeI(TopNode());
  }

  unsigned ExtractKey() {
    ShiftPriority();
    auto node = TopNode();
    RemoveNodeI(node);
    return Key(node);
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
    for (; PNodeAdj(top_priority_adj)->next == PNodeAdj(top_priority_adj);
         ++top_priority)
      top_priority_adj = (top_priority_adj + 1) % window;
  }

  TNode* TopNode() { return PNodeAdj(top_priority_adj)->next; }

  void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      auto knode = KNode(key), pnode = PNode(p);
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
      ++size;
    }
  }

  void SetI(unsigned key, unsigned new_priority) {
    if (priority[key] != new_priority) {
      priority[key] = new_priority;
      auto knode = KNode(key), pnode = PNode(new_priority);
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
