#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"

#include <vector>

namespace heap {
namespace monotone {
namespace ukvm {
// Radix queue with double linked list
// P - max priority, W - window
// Memory  -- O(N + log W)
// Add     -- O(log W)
// DecV    -- O(1) amortized
// IncV    -- O(log W)
// Top     -- O(1) amortized
// Pop     -- O(1) amortized
class RadixWDLL {
 public:
  using TValue = unsigned;
  using TData = heap::ukvm::Data<TValue>;
  using TSelf = RadixWDLL;

  class TNode {
   public:
    unsigned index;
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  std::vector<TNode> nodes;
  std::vector<unsigned> priority;
  std::vector<unsigned> vfirst, vlength;
  TNode *pkey0, *pindex0, *pcurrent;
  unsigned max_index;
  unsigned size;

 protected:
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }
  unsigned Key(const TNode* node) const { return node - pkey0; }
  TNode* INode(unsigned index) { return pindex0 + index; }

  unsigned Index(unsigned p, unsigned l) {
    for (; vfirst[l] > p;) --l;
    return l;
  }

  unsigned Index(unsigned p) { return Index(p, max_index); }

 public:
  void Reset(unsigned ukey_size, unsigned window) {
    priority.clear();
    priority.resize(ukey_size, -1u);
    vfirst.clear();
    vfirst.push_back(0);
    vfirst.push_back(1);
    vlength.resize(2, 1);
    window = std::max(window, 2u);
    for (; vlength.back() < window;) {
      vfirst.push_back(vfirst.back() + vlength.back());
      vlength.push_back(vlength.back() * 2);
    }
    max_index = vlength.size() - 1;
    nodes.clear();
    nodes.resize(ukey_size + max_index + 1);
    pkey0 = &(nodes[0]);
    pindex0 = pkey0 + ukey_size;
    for (unsigned i = 0; i <= max_index; ++i) {
      auto node = INode(i);
      node->next = node->prev = node;
    }
    pcurrent = INode(0);
    size = 0;
  }

  RadixWDLL(unsigned ukey_size, unsigned window) { Reset(ukey_size, window); }

  RadixWDLL(const std::vector<unsigned>& v, bool skip_heap, unsigned window) {
    Reset(v.size(), window);
    priority = v;
    if (!skip_heap) {
      for (unsigned i = 0; i < v.size(); ++i) {
        unsigned p = v[i];
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
  void ShiftPriority() {
    assert(!Empty());
    for (; pcurrent->next == pcurrent;) ++pcurrent;
    if (pcurrent - pindex0 < 2) return;
    if (pcurrent->next == pcurrent->prev) return;
    auto tnode = pcurrent->next;
    unsigned minp = priority[Key(tnode)];
    for (tnode = tnode->next; tnode != pcurrent; tnode = tnode->next)
      minp = std::min(minp, priority[Key(tnode)]);
    vfirst[0] = minp;
    unsigned current_index = pcurrent - pindex0;
    for (unsigned i = 0; i < current_index; ++i)
      vfirst[i + 1] = vfirst[i] + vlength[i];
    for (auto node = pcurrent->next; node != pcurrent; node = pcurrent->next)
      MoveI(node, Index(priority[Key(node)], current_index - 1));
    pcurrent = pindex0;
  }

  TNode* TopNode() { return pcurrent->next; }

  void AddNewKeyI(unsigned key, unsigned p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
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
