#pragma once

#include "common/base.h"
#include "common/heap/ukvm/data.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace heap {
namespace ukvm {
template <class TTSet>
class ProxySet {
 public:
  using TSet = TTSet;
  using TValue = typename TSet::TValue;
  using TData = Data<TValue>;
  using TSelf = ProxySet<TSet>;

  class TNode : public memory::Node {
   public:
    TNode *next = nullptr, *prev = nullptr;
  };

 protected:
  TSet s;
  std::vector<TNode> nodes_key;
  memory::NodesManager<TNode> manager_priority;
  std::vector<TValue> priority;
  std::unordered_map<TValue, TNode*> queue;
  TNode* pkey0;
  unsigned size;

 protected:
  TNode* KNode(unsigned key) { return pkey0 + key; }
  const TNode* KNode(unsigned key) const { return pkey0 + key; }
  TNode* VNode(const TValue& v) { return queue[v]; }
  const TNode* VNode(const TValue& v) const { return queue.find(v)->second; }
  unsigned Key(const TNode* node) const { return node - pkey0; }

 public:
  void Reset(unsigned ukey_size) {
    nodes_key.clear();
    nodes_key.resize(ukey_size);
    manager_priority.ResetNodes();
    priority.clear();
    priority.resize(ukey_size, -1u);
    queue.clear();
    pkey0 = &(nodes_key[0]);
    size = 0;
  }

  explicit ProxySet(unsigned ukey_size) { Reset(ukey_size); }

  ProxySet(const TSet& _s, unsigned ukey_size) : s(_s) { Reset(ukey_size); }

  ProxySet(const std::vector<TValue>& v, bool skip_heap) {
    Reset(v.size());
    if (!skip_heap) AddAllToHeap(v);
  }

  ProxySet(const TSet& _s, const std::vector<TValue>& v, bool skip_heap)
      : s(_s) {
    Reset(v.size());
    if (!skip_heap) AddAllToHeap(v);
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(priority.size()); }
  bool InHeap(unsigned key) const { return KNode(key)->next; }
  TValue Get(unsigned key) const { return priority[key]; }
  const std::vector<TValue>& GetValues() const { return priority; }

 public:
  void AddNewKey(unsigned key, TValue _priority, bool skip_heap = false) {
    assert(!InHeap(key));
    AddNewKeyI(key, _priority, skip_heap);
  }

  void Set(unsigned key, TValue new_priority) {
    if (InHeap(key))
      SetI(key, new_priority);
    else
      AddNewKeyI(key, new_priority, false);
  }

  void DecreaseValue(unsigned key, TValue new_priority) {
    Set(key, new_priority);
  }

  void DecreaseValueIfLess(unsigned key, TValue new_priority) {
    if (new_priority < priority[key]) Set(key, new_priority);
  }

  void IncreaseValue(unsigned key, TValue new_priority) {
    Set(key, new_priority);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() const { return Key(TopNode()); }

  TValue TopValue() const { return s.MinValue(); }

  TData Top() const { return {TopKey(), TopValue()}; }

  void Pop() { RemoveNodeI(TopNode()); }

  unsigned ExtractKey() {
    auto node = TopNode();
    RemoveNodeI(node);
    return Key(node);
  }

  TValue ExtractValue() {
    TValue t = TopValue();
    Pop();
    return t;
  }

  TData Extract() {
    TData t = Top();
    Pop();
    return t;
  }

  void DeleteKey(unsigned key) { RemoveNodeI(KNode(key)); }

 protected:
  TNode* TopNode() { return VNode(s.MinValue())->next; }
  const TNode* TopNode() const { return VNode(s.MinValue())->next; }

  void AddNewKeyI(unsigned key, TValue p, bool skip_heap) {
    priority[key] = p;
    if (!skip_heap) {
      TNode* pnode;
      auto it = queue.find(p);
      if (it == queue.end()) {
        pnode = manager_priority.New();
        pnode->next = pnode->prev = pnode;
        queue[p] = pnode;
        s.Insert(p);
      } else {
        pnode = it->second;
      }
      auto knode = KNode(key);
      knode->prev = pnode->prev;
      knode->prev->next = knode;
      pnode->prev = knode;
      knode->next = pnode;
      ++size;
    }
  }

  void SetI(unsigned key, TValue new_priority) {
    if (priority[key] != new_priority) {
      DeleteKey(key);
      AddNewKeyI(key, new_priority, false);
    }
  }

  void RemoveNodeI(TNode* node) {
    if (node->next == node->prev) {
      const TValue& p = Get(Key(node));
      auto it = queue.find(p);
      assert(it != queue.end());
      manager_priority.Release(it->second);
      queue.erase(it);
      s.Remove(p);
    } else {
      node->next->prev = node->prev;
      node->prev->next = node->next;
    }
    node->prev = node->next = nullptr;
    --size;
  }

  void AddAllToHeap(const std::vector<TValue>& v) {
    for (unsigned i = 0; i < v.size(); ++i) AddNewKeyI(i, v[i], false);
  }
};
}  // namespace ukvm
}  // namespace heap
