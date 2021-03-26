#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager_fixed_size.h"

#include <functional>

namespace heap {
template <class TTValue, class TTCompare = std::less<TTValue>>
class BinomialUKeyValueMap {
 public:
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TSelf = BinomialUKeyValueMap<TValue, TCompare>;

  struct TData {
    unsigned key;
    TValue value;
  };

  struct TPositionValue;

  class Node : public BaseNode {
   public:
    TPositionValue* pv;
    Node *p, *l, *s;
    unsigned d;

    Node() : pv(nullptr) { Clear(); }
    Node(TPositionValue* _pv) : pv(_pv) { Clear(); }

    void Clear() {
      p = l = s = nullptr;
      d = 0;
    }

    void ClearReuse() { d = 0; }
  };

  using TNodesManager = NodesManagerFixedSize<Node>;

  struct TPositionValue {
    Node* heap_position;
    TValue value;
  };

 protected:
  TCompare compare;
  TNodesManager nodes_manager;
  Node* head;
  unsigned size;
  mutable Node* top;
  std::vector<TPositionValue> key_map;
  TPositionValue* pv_begin;

 protected:
  bool Compare(const TPositionValue* pvl, const TPositionValue* pvr) const {
    return compare(pvl->value, pvr->value);
  }

  bool Compare(const Node* l, const Node* r) const {
    return Compare(l->pv, r->pv);
  }

 public:
  explicit BinomialUKeyValueMap(unsigned ukey_size)
      : nodes_manager(ukey_size), head(nullptr), size(0), top(nullptr) {
    key_map.resize(ukey_size, {nullptr, TValue()});
    pv_begin = &(key_map[0]);
  }

  BinomialUKeyValueMap(const std::vector<TValue>& v, bool skip_heap)
      : BinomialUKeyValueMap(v.size()) {
    unsigned n = UKeySize();
    for (unsigned i = 0; i < n; ++i) AddNewKeyI(i, v[i], skip_heap);
  }

  bool Empty() const { return !head; }
  unsigned Size() const { return size; }
  unsigned UKeySize() const { return unsigned(key_map.size()); }

  const TValue& Get(unsigned key) const { return key_map[key].value; }

  std::vector<TValue> GetValues() const {
    unsigned n = UKeySize();
    std::vector<TValue> v(n);
    for (unsigned i = 0; i < n; ++i) v[i] = key_map[i].value;
    return v;
  }

 protected:
  void AddNewKeyI(unsigned key, const TValue& new_value, bool skip_heap) {
    key_map[key].value = new_value;
    if (!skip_heap) {
      Node* pv = nodes_manager.New();
      pv->pv = pv_begin + key;
      key_map[key].heap_position = pv;
      if (top && Compare(pv, top)) top = pv;
      AddOneNode(pv);
      ++size;
    }
  }

  void DecreaseValueI(unsigned key, Node* node, const TValue& new_value) {
    key_map[key].value = new_value;
    SiftUp(node);
  }

  void IncreaseValueI(unsigned key, Node* node, const TValue& new_value) {
    key_map[key].value = new_value;
    SiftDown(node);
  }

 public:
  void AddNewKey(unsigned key, const TValue& new_value,
                 bool skip_heap = false) {
    assert(!key_map[key].heap_position);
    AddNewKeyI(key, new_value, skip_heap);
  }

  void DecreaseValue(unsigned key, const TValue& new_value) {
    Node* node = key_map[key].heap_position;
    if (!node)
      AddNewKeyI(key, new_value, false);
    else
      DecreaseValueI(key, node, new_value);
  }

  void DecreaseValueIfLess(unsigned key, const TValue& new_value) {
    if (compare(new_value, key_map[key].value)) DecreaseValue(key, new_value);
  }

  void IncreaseValue(unsigned key, const TValue& new_value) {
    Node* node = key_map[key].heap_position;
    assert(node);
    IncreaseValueI(key, node, new_value);
  }

  void Set(unsigned key, const TValue& new_value) {
    Node* node = key_map[key].heap_position;
    if (!node)
      AddNewKeyI(key, new_value, false);
    else if (compare(new_value, key_map[key].value))
      DecreaseValueI(key, node, new_value);
    else
      IncreaseValueI(key, node, new_value);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  const Node* TopNode() const {
    assert(!Empty());
    if (!top) SetTopNode();
    return top;
  }

  unsigned TopKey() const { return TopNode()->pv - pv_begin; }
  const TValue& TopValue() const { return TopNode()->pv->value; }

  TData Top() const {
    TPositionValue* pv = TopNode()->pv;
    return {pv - pv_begin, pv->value};
  }

  void Delete(Node* node) {
    assert(node);
    if (top == node) ResetTopNode();
    node = ForceSiftUp(node);
    CutNode(node);
    node->pv->heap_position = nullptr;
    nodes_manager.Release(node);
    --size;
  }

  void Pop() { Delete(TopNode()); }

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

 protected:
  void ResetTopNode() const { top = nullptr; }

  void UpdateTopNode() const {
    // It's possible that after compress top node will be under node with same
    // value.
    if (top) {
      for (; top->p;) top = top->p;
    }
  }

  void SetTopNode() const {
    top = head;
    for (Node* c = top->s; c; c = c->s) {
      if (Compare(c, top)) top = c;
    }
  }

  Node* TopNode() {
    assert(!Empty());
    if (!top) SetTopNode();
    return top;
  }

  static void Link(Node* x, Node* y) {
    x->p = y;
    x->s = y->l;
    y->l = x;
    ++y->d;
  }

  void AddOneNode(Node* p) {
    p->s = head;
    head = p;
    for (Node* ps = p->s; ps && (p->d == ps->d); ps = p->s) {
      if (Compare(p, ps)) {
        p->s = ps->s;
        Link(ps, p);
      } else {
        head = ps;
        Link(p, ps);
        p = ps;
      }
    }
    UpdateTopNode();
  }

  static Node* Merge(Node* h1, Node* h2) {
    if (!h1) return h2;
    if (!h2) return h1;
    Node* r;
    if (h1->d < h2->d) {
      r = h1;
      h1 = h1->s;
    } else {
      r = h2;
      h2 = h2->s;
    }
    Node* c = r;
    for (; h1 && h2; c = c->s) {
      if (h1->d < h2->d) {
        c->s = h1;
        h1 = h1->s;
      } else {
        c->s = h2;
        h2 = h2->s;
      }
    }
    c->s = (h1 ? h1 : h2);
    return r;
  }

  void Compress() {
    if (Empty()) return;
    Node *pp = nullptr, *pc = head, *pn = pc->s;
    for (; pn; pn = pc->s) {
      if ((pc->d != pn->d) || (pn->s && (pn->d == pn->s->d))) {
        pp = pc;
        pc = pn;
      } else {
        if (Compare(pc, pn)) {
          pc->s = pn->s;
          Link(pn, pc);
        } else {
          if (pp)
            pp->s = pn;
          else
            head = pn;
          Link(pc, pn);
          pc = pn;
        }
      }
    }
    UpdateTopNode();
  }

  void Union(Node* rhead) {
    head = Merge(head, rhead);
    Compress();
  }

  void CutTree(Node* node) {
    if (head == node)
      head = node->s;
    else {
      Node* c = head;
      for (; c->s != node;) c = c->s;
      c->s = node->s;
    }
    node->s = nullptr;
  }

  Node* CutChildren(Node* node) {
    Node* c = node->l;
    node->l = nullptr;
    if (!c) return c;
    Node *pc = nullptr, *nc = c->s;
    for (; nc; nc = c->s) {
      c->p = nullptr;
      c->s = pc;
      pc = c;
      c = nc;
    }
    c->p = nullptr;
    c->s = pc;
    return c;
  }

  void CutNode(Node* node) {
    assert(node && !node->p);
    CutTree(node);
    Union(CutChildren(node));
  }

  Node* ForceSiftUp(Node* node) {
    if (!node->p) return node;
    TPositionValue* pv = node->pv;
    for (; node->p; node = node->p) {
      node->pv = node->p->pv;
      node->pv->heap_position = node;
    }
    node->pv = pv;
    pv->heap_position = node;
    if (top == node) ResetTopNode();
    return node;
  }

  void SiftUp(Node* node) {
    if (node->p) {
      if (!Compare(node, node->p)) return;
      TPositionValue* pv = node->pv;
      node->pv = node->p->pv;
      node->pv->heap_position = node;
      for (node = node->p; node->p && Compare(pv, node->p->pv);
           node = node->p) {
        node->pv = node->p->pv;
        node->pv->heap_position = node;
      }
      node->pv = pv;
      pv->heap_position = node;
    }
    if (top && !node->p && Compare(node, top)) top = node;
  }

  void SiftDown(Node* node) {
    node = ForceSiftUp(node);
    CutNode(node);
    Union(node);
  }
};
}  // namespace heap
