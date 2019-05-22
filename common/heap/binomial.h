#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include <functional>

namespace heap {
template <class TTData, class TTCompare = std::less<TTData>,
          template <class TNode> class TTNodesManager = NodesManager>
class Binomial {
 public:
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf = Binomial<TData, TCompare>;

  class Node : public BaseNode {
   public:
    TData value;
    Node *l, *s;
    unsigned d;

    Node() { Clear(); }
    Node(const TData& _value) : value(_value) { Clear(); }

    void Clear() {
      l = s = nullptr;
      d = 0;
    }

    void ClearReuse() { d = 0; }
  };

  using TNodesManager = TTNodesManager<Node>;

 protected:
  TCompare compare;
  TNodesManager& nodes_manager;
  Node* head;
  unsigned size;
  mutable Node* top;

  bool Compare(Node* l, Node* r) const { return compare(l->value, r->value); }

 public:
  Binomial(TNodesManager& _nodes_manager)
      : nodes_manager(_nodes_manager), head(nullptr), top(nullptr), size(0) {}

  TSelf Make() const { return TSelf(nodes_manager); }
  bool Empty() const { return !head; }
  unsigned Size() const { return size; }

  void Add(const TData& v) {
    Node* pv = nodes_manager.New(v);
    if (top && Compare(pv, top)) top = pv;
    AddOneNode(pv);
    ++size;
  }

  Node* TopNode() const {
    assert(!Empty());
    if (!top) SetTopNode();
    return top;
  }

  const TData& Top() const { return TopNode()->value; }

  void Pop() { DeleteI(TopNode()); }

  TData Extract() {
    TData v = Top();
    Pop();
    return v;
  }

  void Union(TSelf& h) {
    assert(&nodes_manager == &(h.nodes_manager));
    if (top && h.top)
      top = Compare(top, h.top) ? top : h.top;
    else
      ResetTopNode();
    Union(h.head);
    h.head = nullptr;
    size += h.size;
    h.size = 0;
    h.ResetTopNode();
  }

 protected:
  void ResetTopNode() const { top = nullptr; }

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
      c->s = pc;
      pc = c;
      c = nc;
    }
    c->s = pc;
    return c;
  }

  void DeleteI(Node* node) {
    assert(node);
    ResetTopNode();
    CutTree(node);
    Union(CutChildren(node));
    nodes_manager.Release(node);
    --size;
  }
};
}  // namespace heap
