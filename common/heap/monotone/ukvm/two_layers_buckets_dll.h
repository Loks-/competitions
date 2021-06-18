#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include "common/nodes_manager_fixed_size.h"

#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// Similar to BucketQueue but use less memory.
// Memory  -- O(N + first_layer_size + P / first_layer_size)
template <unsigned first_layer_size>
class TwoLayersBucketsDLL {
 public:
  static const unsigned fl_size = first_layer_size;
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = TwoLayersBucketsDLL<fl_size>;

  class TNode : public BaseNode {
   public:
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  NodesManagerFixedSize<TNode> manager1;
  NodesManager<TNode> manager2;
  TNode *pkey0 = nullptr, *ppriority0 = nullptr;
  std::vector<TNode*> priority_map;  // Second queue only
  std::vector<unsigned> priorities;
  unsigned top_priority = 0, p1b = 0, p1e = fl_size;
  unsigned size = 0;

 protected:
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }

  TNode* PNodeQ1(unsigned priority) { return ppriority0 + (priority - p1b); }
  TNode* PNodeQ2(unsigned priority) { return priority_map[priority / fl_size]; }

  TNode* PNode(unsigned priority) {
    return (priority < p1e) ? PNodeQ1(priority) : PNodeQ2(priority);
  }

  unsigned Key(const TNode* node) const { return node - pkey0; }

 public:
  void Reset(unsigned ukey_size) {
    manager1.Reset(ukey_size + fl_size);
    pkey0 = manager1.NodeByRawIndex(0);
    ppriority0 = manager1.NodeByRawIndex(ukey_size);
    manager2.ResetNodes();
    priority_map.clear();
    priorities.resize(ukey_size);
    top_priority = p1b = size = 0;
    p1e = fl_size;
    for (unsigned i = 0; i < fl_size; ++i) {
      auto n = PNode(i);
      n->next = n->prev = n;
    }
  }

  explicit TwoLayersBucketsDLL(unsigned ukey_size) { Reset(ukey_size); }

  TwoLayersBucketsDLL(const std::vector<unsigned>& v, bool skip_heap) {
    Reset(v.size());
    if (skip_heap) {
      for (unsigned i = 0; i < priorities.size(); ++i) priorities[i] = v[i];
    } else {
      for (unsigned i = 0; i < priorities.size(); ++i) {
        unsigned p = v[i];
        priorities[i] = p;
        AdjustQueueSize(p);
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
    AddNewKeyI(key, priority, skip_heap);
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
    if (new_priority < priorities[key]) Set(key, new_priority);
  }

  void IncreaseValue(unsigned key, unsigned new_priority) {
    Set(key, new_priority);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() {
    ShiftPriority();
    return Key(PNodeQ1(top_priority)->next);
  }

  unsigned TopValue() {
    ShiftPriority();
    return top_priority;
  }

  TData Top() { return {TopKey(), TopValue()}; }

  void Pop() {
    ShiftPriority();
    RemoveNodeI(PNodeQ1(top_priority)->next);
  }

  unsigned ExtractKey() {
    ShiftPriority();
    auto n = PNodeQ1(top_priority)->next;
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
  void AdjustQueueSize(unsigned k) {
    unsigned s = priority_map.size();
    if (s <= k / fl_size) {
      priority_map.resize(k / fl_size + 1);
      for (; s < priority_map.size(); ++s) {
        auto n = manager2.New();
        n->next = n->prev = n;
        priority_map[s] = n;
      }
    }
  }

  void ShiftPriority() {
    assert(!Empty());
    for (;;) {
      for (; top_priority < p1e; ++top_priority) {
        auto n = PNodeQ1(top_priority);
        if (n->next != n) return;
      }
      p1b = p1e;
      p1e += fl_size;
      auto n = PNodeQ2(p1b);
      for (; n->next != n;) {
        auto knode = n->next;
        n->next = knode->next;
        auto pnode = PNodeQ1(priorities[Key(knode)]);
        knode->prev = pnode->prev;
        knode->prev->next = knode;
        pnode->prev = knode;
        knode->next = pnode;
      }
      n->prev = n;
    }
  }

  void AddNewKeyI(unsigned key, unsigned priority, bool skip_heap) {
    priorities[key] = priority;
    if (!skip_heap) {
      AdjustQueueSize(priority);
      auto knode = KNode(key), pnode = PNode(priority);
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
      ++size;
    }
  }

  void SetI(unsigned key, unsigned new_priority) {
    unsigned p = priorities[key];
    priorities[key] = new_priority;
    bool update_required = (new_priority < p1e)
                               ? (new_priority != p)
                               : (new_priority / fl_size != p / fl_size);
    if (update_required) {
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
