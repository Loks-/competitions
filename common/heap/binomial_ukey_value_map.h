#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"
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

  class Node : public BaseNode {
   public:
    unsigned key;
    Node *p, *l, *s;
    unsigned d;

    Node() { Clear(); }
    Node(unsigned _key) : key(_key) { Clear(); }

    void Clear() {
      p = l = s = 0;
      d = 0;
    }

    void ClearReuse() { d = 0; }
  };

  using TNodesManager = NodesManager<Node>;

 protected:
  TCompare compare;
  TNodesManager nodes_manager;
  Node* head;
  std::vector<Node*> heap_position;
  std::vector<TValue> values;
  mutable Node* top;

  bool Compare(unsigned lkey, unsigned rkey) const {
    return compare(values[lkey], values[rkey]);
  }

  bool Compare(Node* l, Node* r) const { return Compare(l->key, r->key); }

  void ResetHeapPosition(unsigned ukey_size) {
    heap_position.clear();
    heap_position.resize(ukey_size);
    values.resize(ukey_size);
  }

 public:
  BinomialUKeyValueMap(unsigned ukey_size)
      : nodes_manager(ukey_size), head(0), top(0) {
    ResetHeapPosition(ukey_size);
  }

  BinomialUKeyValueMap(const std::vector<TValue>& v, bool skip_heap)
      : nodes_manager(v.size()), head(0), values(v), top(0) {
    if (skip_heap) {
      ResetHeapPosition(v.size());
    } else {
      assert(false);
      // heap_position = heap_keys = Enumerate<unsigned>(0, v.size());
      // Heapify();
    }
  }

  bool Empty() const { return !head; }
  unsigned UKeySize() const { return unsigned(values.size()); }

  const TValue& Get(unsigned key) const { return values[key]; }
  const std::vector<TValue>& GetValues() const { return values; }

 protected:
  void AddNewKeyI(unsigned key, const TValue& new_value, bool skip_heap) {
    values[key] = new_value;
    if (!skip_heap) {
      Node* pv = nodes_manager.New(key);
      heap_position[key] = pv;
      if (top && Compare(pv, top)) top = pv;
      Union(pv);
    }
  }

  void DecreaseValueI(unsigned key, Node* node, const TValue& new_value) {
    values[key] = new_value;
    SiftUp(node);
  }

  void IncreaseValueI(unsigned key, Node* node, const TValue& new_value) {
    values[key] = new_value;
    SiftDown(node);
  }

 public:
  void AddNewKey(unsigned key, const TValue& new_value,
                 bool skip_heap = false) {
    assert(!heap_position[key]);
    AddNewKeyI(key, new_value, skip_heap);
  }

  void DecreaseValue(unsigned key, const TValue& new_value) {
    Node* node = heap_position[key];
    if (!node)
      AddNewKeyI(key, new_value, false);
    else
      DecreaseValueI(key, node, new_value);
  }

  void DecreaseValueIfLess(unsigned key, const TValue& new_value) {
    if (compare(new_value, values[key])) {
      Node* node = heap_position[key];
      if (!node)
        AddNewKeyI(key, new_value, false);
      else
        DecreaseValueI(key, node, new_value);
    }
  }

  void IncreaseValue(unsigned key, const TValue& new_value) {
    Node* node = heap_position[key];
    assert(node);
    IncreaseValueI(key, node, new_value);
  }

  void Set(unsigned key, const TValue& new_value) {
    Node* node = heap_position[key];
    if (!node)
      AddNewKeyI(key, new_value, false);
    else if (compare(new_value, values[key]))
      DecreaseValueI(key, node, new_value);
    else
      IncreaseValueI(key, node, new_value);
  }

  void Add(const TData& x) { Set(x.key, x.value); }

  Node* TopNode() const {
    assert(!Empty());
    if (!top) SetTopNode();
    return top;
  }

  unsigned TopKey() const { return TopNode()->key; }
  const TValue& TopValue() const { return Get(TopKey()); }

  TData Top() const {
    unsigned key = TopKey();
    return {key, Get(key)};
  }

  void Delete(Node* node) {
    assert(node);
    if (top == node) ResetTopNode();
    node = ForceSiftUp(node);
    CutNode(node);
    heap_position[node->key] = 0;
    nodes_manager.Release(node);
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
  void ResetTopNode() const { top = 0; }

  void SetTopNode() const {
    top = head;
    for (Node* c = top->s; c; c = c->s) {
      if (Compare(c, top)) top = c;
    }
  }

  static void Link(Node* x, Node* y) {
    x->p = y;
    x->s = y->l;
    y->l = x;
    ++y->d;
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
    Node *pp = 0, *pc = head, *pn = pc->s;
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
  }

  void Heapify() {
    if (Empty()) return;
    Node* l = head;
    for (; l->s;) l = l->s;
    Node *pp = 0, *pc = head, *pn = pc->s;
    for (; pn; pn = pc->s) {
      if (pc->d == pn->d) {
        if (pp)
          pp->s = pn->s;
        else
          head = pn->s;
        if (Compare(pc, pn)) {
          Link(pn, pc);
          pc->s = 0;
          if (l != pn) {
            l->s = pc;
            l = pc;
          }
        } else {
          Link(pc, pn);
          pn->s = 0;
          if (l != pn) {
            l->s = pn;
            l = pn;
          }
        }
        pc = pp ? pp->s : head;
      } else {
        pp = pc;
        pc = pn;
      }
    }
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
    node->s = 0;
  }

  Node* CutChildren(Node* node) {
    Node* c = node->l;
    node->l = 0;
    if (!c) return c;
    Node *pc = 0, *nc = c->s;
    for (; nc; nc = c->s) {
      c->p = 0;
      c->s = pc;
      pc = c;
      c = nc;
    }
    c->p = 0;
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
    unsigned key = node->key;
    for (; node->p; node = node->p) {
      node->key = node->p->key;
      heap_position[node->key] = node;
    }
    node->key = key;
    heap_position[key] = node;
    if (top == node) ResetTopNode();
    return node;
  }

  void SiftUp(Node* node) {
    if (!node->p || !Compare(node, node->p)) return;
    unsigned key = node->key;
    node->key = node->p->key;
    heap_position[node->key] = node;
    for (node = node->p; node->p && Compare(node, node->p); node = node->p) {
      node->key = node->p->key;
      heap_position[node->key] = node;
    }
    node->key = key;
    heap_position[key] = node;
    if (top && !node->p && Compare(node, top)) top = node;
  }

  void SiftDown(Node* node) {
    node = ForceSiftUp(node);
    CutNode(node);
    Union(node);
  }
};
}  // namespace heap
