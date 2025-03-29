#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// Radix queue with double linked list
// P - max priority
// Memory  -- O(N + log P)
// Add     -- O(log P)
// DecV    -- O(1) amortized
// IncV    -- O(log P)
// Top     -- O(1) amortized
// Pop     -- O(1) amortized
class RadixDLL {
 public:
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = RadixDLL;

  class TNode : public memory::Node {
   public:
    unsigned index;
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  std::vector<TNode> nodes_key;
  memory::NodesManager<TNode> manager_priority;
  std::vector<unsigned> priority;
  std::vector<TNode*> queue;
  std::vector<unsigned> vfirst, vlength;
  TNode* pkey0;
  unsigned current_index;
  unsigned size;

 protected:
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }
  unsigned Key(const TNode* node) const { return node - pkey0; }
  TNode* INode(unsigned index) { return queue[index]; }

  unsigned Index(unsigned p, unsigned l) {
    for (; vfirst[l] > p;) --l;
    return l;
  }

  unsigned Index(unsigned p) { return Index(p, queue.size() - 1); }

 public:
  void Reset(unsigned ukey_size) {
    nodes_key.clear();
    nodes_key.resize(ukey_size);
    manager_priority.clear();
    priority.clear();
    priority.resize(ukey_size, -1u);
    queue.clear();
    pkey0 = &(nodes_key[0]);
    vfirst.clear();
    vfirst.push_back(0);
    vfirst.push_back(1);
    vfirst.push_back(2);
    vlength.resize(2, 1);
    for (unsigned i = 0; i < 2; ++i) {
      auto node = manager_priority.create();
      node->next = node->prev = node;
      queue.push_back(node);
    }
    current_index = 0;
    size = 0;
  }

  explicit RadixDLL(unsigned ukey_size) { Reset(ukey_size); }

  RadixDLL(const std::vector<unsigned>& v, bool skip_heap) {
    Reset(v.size());
    priority = v;
    if (!skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
        AdjustQueueSize(p);
        unsigned index = Index(p);
        auto knode = KNode(i), pnode = INode(index);
        knode->index = index;
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
    if (!InHeap(key))
      AddNewKeyI(key, new_priority, false);
    else if (priority[key] < new_priority)
      IncreaseI(key, new_priority);
    else
      DecreaseI(key, new_priority);
  }

  void DecreaseValue(unsigned key, unsigned new_priority) {
    if (InHeap(key))
      DecreaseI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  void DecreaseValueIfLess(unsigned key, unsigned new_priority) {
    if (new_priority < priority[key]) DecreaseValue(key, new_priority);
  }

  void IncreaseValue(unsigned key, unsigned new_priority) {
    if (InHeap(key))
      IncreaseI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() {
    ShiftPriority();
    return Key(TopNode());
  }

  unsigned TopValue() { return priority[TopKey()]; }

  TData Top() {
    auto key = TopKey();
    return {key, priority[key]};
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
    unsigned key = ExtractKey();
    return priority[key];
  }

  TData Extract() {
    unsigned key = ExtractKey();
    return {key, priority[key]};
  }

  void DeleteKey(unsigned key) { RemoveNodeI(KNode(key)); }

 protected:
  void AdjustQueueSize(unsigned p) {
    for (; vfirst.back() <= p;) {
      vlength.push_back(vlength.back() * 2);
      vfirst.push_back(vfirst.back() + vlength.back());
      auto n = manager_priority.create();
      n->next = n->prev = n;
      queue.push_back(n);
    }
  }

  void ShiftPriority() {
    assert(!Empty());
    for (; queue[current_index]->next == queue[current_index];) ++current_index;
    if (current_index < 2) return;
    auto pnode = queue[current_index];
    if (pnode->next == pnode->prev) return;
    auto tnode = pnode->next;
    unsigned minp = priority[Key(tnode)];
    for (tnode = tnode->next; tnode != pnode; tnode = tnode->next)
      minp = std::min(minp, priority[Key(tnode)]);
    vfirst[0] = minp;
    for (unsigned i = 0; i < current_index; ++i)
      vfirst[i + 1] = vfirst[i] + vlength[i];
    for (auto node = pnode->next; node != pnode; node = pnode->next)
      MoveI(node, Index(priority[Key(node)], current_index - 1));
    current_index = 0;
  }

  TNode* TopNode() { return queue[current_index]->next; }

  void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      AdjustQueueSize(p);
      unsigned index = Index(p);
      auto knode = KNode(key), pnode = INode(index);
      knode->index = index;
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
      ++size;
    }
  }

  void MoveI(TNode* knode, unsigned new_index) {
    auto pnode = INode(new_index);
    knode->index = new_index;
    knode->next->prev = knode->prev;
    knode->prev->next = knode->next;
    knode->prev = pnode->prev;
    knode->prev->next = knode;
    pnode->prev = knode;
    knode->next = pnode;
  }

  void DecreaseI(unsigned key, unsigned new_priority) {
    priority[key] = new_priority;
    auto node = KNode(key);
    unsigned new_index = Index(new_priority, node->index);
    if (new_index != node->index) MoveI(node, new_index);
  }

  void IncreaseI(unsigned key, unsigned new_priority) {
    priority[key] = new_priority;
    auto node = KNode(key);
    unsigned new_index = Index(new_priority);
    if (new_index != node->index) MoveI(node, new_index);
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
