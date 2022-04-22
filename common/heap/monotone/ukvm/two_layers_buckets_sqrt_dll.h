#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
#include "common/numeric/bits/ulog2.h"

#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// P - max priority
// Memory  -- O(N + sqrt(P))
// Add     -- O(1)
// DecV    -- O(1)
// IncV    -- O(1)
// Top     -- O(1 + P / N) amortized
// Pop     -- O(1 + P / N) amortized
class TwoLayersBucketsSqrtDLL {
 public:
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = TwoLayersBucketsSqrtDLL;

  class TNode : public memory::Node {
   public:
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  std::vector<TNode> nodes;
  memory::NodesManager<TNode> manager;
  std::vector<unsigned> priority;
  std::vector<TNode*> queue2;  // Second queue only
  TNode *pkey0, *ppriority0;
  unsigned top_priority, lq1size, q1size, p1b, p1e;
  unsigned size;

 protected:
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }
  TNode* PNodeQ1(unsigned p) { return ppriority0 + (p - p1b); }
  TNode* PNodeQ2(unsigned p) { return queue2[p >> lq1size]; }
  TNode* PNode(unsigned p) { return (p < p1e) ? PNodeQ1(p) : PNodeQ2(p); }
  unsigned Key(const TNode* node) const { return node - pkey0; }

 public:
  void Reset(unsigned ukey_size, unsigned expected_max_priority) {
    top_priority = 0;
    lq1size = numeric::ULog2(expected_max_priority) / 2 + 1;
    q1size = (1u << lq1size);
    p1b = 0;
    p1e = q1size;
    nodes.clear();
    nodes.resize(ukey_size + q1size);
    manager.ResetNodes();
    priority.clear();
    priority.resize(ukey_size, -1u);
    queue2.clear();
    pkey0 = &(nodes[0]);
    ppriority0 = pkey0 + ukey_size;
    size = 0;
    for (unsigned i = 0; i < q1size; ++i) {
      auto n = PNodeQ1(i);
      n->next = n->prev = n;
    }
  }

  TwoLayersBucketsSqrtDLL(unsigned ukey_size, unsigned expected_max_priority) {
    Reset(ukey_size, expected_max_priority);
  }

  TwoLayersBucketsSqrtDLL(const std::vector<unsigned>& v, bool skip_heap,
                          unsigned expected_max_priority) {
    Reset(v.size(), expected_max_priority);
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
    p >>= lq1size;
    unsigned s = queue2.size();
    if (s <= p) {
      queue2.resize(p + 1);
      for (; s <= p; ++s) {
        auto n = manager.New();
        n->next = n->prev = n;
        queue2[s] = n;
      }
    }
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; top_priority < p1e; ++top_priority) {
      auto n = PNodeQ1(top_priority);
      if (n->next != n) return;
    }
    for (p1b = p1e;; p1b += q1size) {
      auto n = PNodeQ2(p1b);
      if (n->next != n) break;
    }
    p1e = p1b + q1size;
    auto n = PNodeQ2(p1b);
    for (; n->next != n;) {
      auto knode = n->next;
      n->next = knode->next;
      auto pnode = PNodeQ1(priority[Key(knode)]);
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
    }
    n->prev = n;
    for (top_priority = p1b;; ++top_priority) {
      auto n = PNodeQ1(top_priority);
      if (n->next != n) return;
    }
  }

  TNode* TopNode() { return PNodeQ1(top_priority)->next; }

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
    unsigned p = priority[key];
    priority[key] = new_priority;
    bool update_required = (p < p1e)
                               ? (new_priority != p)
                               : ((new_priority >> lq1size) != (p >> lq1size));
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
