#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include <functional>

namespace heap {
template <class TTValue, class TTCompare = std::less<TTValue>>
class Binomial {
 public:
  using TValue = TTValue;
  using TCompare = TTCompare;
  using TSelf = Binomial<TValue, TCompare>;

  class Node : public BaseNode {
   public:
    TValue value;
    Node *l, *s;
    unsigned d;

    Node() { Clear(); }
    Node(const TValue& _value) : value(_value) { Clear(); }

    void Clear() {
      l = s = 0;
      d = 0;
    }

    void ClearReuse() { d = 0; }
  };

  using TNodesManager = NodesManager<Node>;

 protected:
  TCompare compare;
  TNodesManager& nodes_manager;
  Node* head;
  mutable Node* top;

  bool Compare(Node* l, Node* r) const { return compare(l->value, r->value); }

 public:
  Binomial(TNodesManager& _nodes_manager)
      : nodes_manager(_nodes_manager), head(0), top(0) {}

  TSelf Make() const { return TSelf(nodes_manager); }
  bool Empty() const { return !head; }

  Node* TopNode() const {
    assert(!Empty());
    if (!top) SetTopNode();
    return top;
  }

  const TValue& Top() const { return TopNode()->value; }

  void Union(TSelf& h) {
    assert(&nodes_manager == &(h.nodes_manager));
    if (top && h.top)
      top = Compare(top, h.top) ? top : h.top;
    else
      ResetTopNode();
    UnionI(h.head);
    h.head = 0;
    h.ResetTopNode();
  }

  void Add(const TValue& v) {
    Node* pv = nodes_manager.New(v);
    if (top && Compare(pv, top)) top = pv;
    UnionI(pv);
  }

  void Delete(Node* node) {
    assert(node && !node->p);
    if (top == node) ResetTopNode();
    CutTree(node);
    UnionI(CutChildren(node));
    nodes_manager.Release(node);
  }

  void Pop() { Delete(TopNode()); }

  TValue Extract() {
    TValue v = Top();
    Pop();
    return v;
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

  void UnionI(Node* rhead) {
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
      c->s = pc;
      pc = c;
      c = nc;
    }
    c->s = pc;
    return c;
  }
};
}  // namespace heap
