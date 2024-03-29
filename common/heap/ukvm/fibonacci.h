#pragma once

#include "common/base.h"
#include "common/heap/ext/fibonacci.h"
#include "common/heap/ukvm/data.h"
#include "common/memory/nodes_manager_fixed_size.h"

#include <functional>
#include <vector>

namespace heap {
namespace ukvm {
// Memory  -- O(N)
// Add     -- O(1)
// DecV    -- O(1) amortized
// IncV    -- O(log N) amortized
// Top     -- O(1)
// Pop     -- O(log N) amortized
// Union   -- O(1)
template <class TTValue, class TCompare = std::less<TTValue>>
class Fibonacci : public heap::ext::Fibonacci<TTValue, TCompare,
                                              memory::NodesManagerFixedSize> {
 public:
  using TValue = TTValue;
  using TData = Data<TValue>;
  using TBase =
      heap::ext::Fibonacci<TValue, TCompare, memory::NodesManagerFixedSize>;
  using TSelf = Fibonacci<TValue, TCompare>;
  using TNode = typename TBase::Node;
  using TNodesManager = typename TBase::TNodesManager;

 protected:
  TNodesManager manager;

  bool UnusedNode(const TNode* p) { return p->d == -1u; }
  void SetUnused(TNode* p) { p->d = -1u; }
  void CleanUnused(TNode* p) { p->d = 0; }

  TNode* GetNode(unsigned key) { return manager.NodeByRawIndex(key); }

 public:
  explicit Fibonacci(unsigned ukey_size) : TBase(manager), manager(ukey_size) {
    for (unsigned i = 0; i < ukey_size; ++i) SetUnused(manager.New());
  }

  Fibonacci(const std::vector<TValue>& v, bool skip_heap)
      : TBase(manager), manager(v.size()) {
    for (unsigned i = 0; i < v.size(); ++i) {
      auto p = manager.New();
      p->value = v[i];
      if (skip_heap)
        SetUnused(p);
      else
        TBase::AddNode(p);
    }
  }

  unsigned UKeySize() const { return manager.Size(); }

  const TNode* GetNode(unsigned key) const {
    return manager.NodeByRawIndex(key);
  }

  bool InHeap(unsigned key) const { return !UnusedNode(GetNode(key)); }

  const TValue& Get(unsigned key) const { return GetNode(key)->value; }

  std::vector<TValue> GetValues() const {
    unsigned n = UKeySize();
    std::vector<TValue> v(n);
    for (unsigned i = 0; i < n; ++i) v[i] = Get(i);
    return v;
  }

  unsigned GetKey(const TNode* node) const { return manager.RawIndex(node); }

 protected:
  void AddNewKeyI(TNode* node, const TValue& new_value, bool skip_heap) {
    node->value = new_value;
    if (!skip_heap) {
      CleanUnused(node);
      TBase::AddNode(node);
    }
  }

 public:
  void AddNewKey(unsigned key, const TValue& new_value,
                 bool skip_heap = false) {
    TNode* node = GetNode(key);
    assert(UnusedNode(node));
    AddNewKeyI(node, new_value, skip_heap);
  }

  void DecreaseValue(unsigned key, const TValue& new_value) {
    TNode* node = GetNode(key);
    if (UnusedNode(node))
      AddNewKeyI(node, new_value, false);
    else
      TBase::DecreaseValue(node, new_value);
  }

  void DecreaseValueIfLess(unsigned key, const TValue& new_value) {
    TNode* node = GetNode(key);
    if (TBase::compare(new_value, node->value)) DecreaseValue(key, new_value);
  }

  void IncreaseValue(unsigned key, const TValue& new_value) {
    TNode* node = GetNode(key);
    assert(!UnusedNode(node));
    TBase::IncreaseValue(node, new_value);
  }

  void Set(unsigned key, const TValue& new_value) {
    TNode* node = GetNode(key);
    if (UnusedNode(node))
      AddNewKeyI(node, new_value, false);
    else if (TBase::compare(new_value, node->value))
      TBase::DecreaseValue(node, new_value);
    else
      TBase::IncreaseValue(node, new_value);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  unsigned TopKey() const { return GetKey(TBase::TopNode()); }
  const TValue& TopValue() const { return TBase::Top(); }

  TData Top() const {
    TNode* node = TBase::TopNode();
    return {GetKey(node), node->value};
  }

  void Delete(TNode* node) {
    TBase::RemoveNode(node);
    SetUnused(node);
  }

  void Pop() { Delete(TBase::TopNode()); }

  unsigned ExtractKey() {
    unsigned t = TopKey();
    Pop();
    return t;
  }

  const TValue& ExtractValue() {
    const TValue& t = TopValue();
    Pop();
    return t;
  }

  TData Extract() {
    TData t = Top();
    Pop();
    return t;
  }
};
}  // namespace ukvm
}  // namespace heap
