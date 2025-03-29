#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// Bucket queue with double linked list
// P - max priority
// Memory  -- O(N + P)
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(1 + P / N) amortized
// Pop     -- O(1 + P / N) amortized
class BucketQueueDLL {
 public:
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
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
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }
  TNode* PNode(unsigned p) { return queue[p]; }
  unsigned Key(const TNode* node) const { return node - pkey0; }

 public:
  void Reset(unsigned ukey_size) {
    nodes_key.clear();
    nodes_key.resize(ukey_size);
    manager_priority.clear();
    priority.clear();
    priority.resize(ukey_size, -1u);
    queue.clear();
    pkey0 = &(nodes_key[0]);
    top_priority = 0;
    size = 0;
  }

  explicit BucketQueueDLL(unsigned ukey_size) { Reset(ukey_size); }

  BucketQueueDLL(const std::vector<unsigned>& v, bool skip_heap) {
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
  void AdjustQueueSize(unsigned p) {
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

  void ShiftPriority() {
    assert(!Empty());
    for (; queue[top_priority]->next == queue[top_priority];) ++top_priority;
  }

  TNode* TopNode() { return PNode(top_priority)->next; }

  void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      AdjustQueueSize(p);
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
