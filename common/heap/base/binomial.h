#pragma once

#include "common/base.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <functional>

namespace heap {
namespace base {
// Memory  -- O(N)
// Add     -- O(1) amortized
// Top     -- O(1) amortized
// Pop     -- O(log N)
// Union   -- O(log N)
template <class TData, class TCompare = std::less<TData>,
          template <class TNode> class TTNodesManager = memory::NodesManager>
class Binomial {
 public:
  class Node : public memory::Node {
   public:
    TData value;
    Node *l, *s;
    unsigned d;

   public:
    constexpr Node() { Clear(); }

    constexpr Node(const TData& _value) : value(_value) { Clear(); }

    constexpr void Clear() {
      l = s = nullptr;
      d = 0;
    }

    constexpr void ClearReuse() { d = 0; }

    constexpr const TData& GetValue() const { return value; }
  };

  using TNodesManager = TTNodesManager<Node>;
  using TSelf = Binomial<TData, TCompare, TTNodesManager>;

 public:
  constexpr explicit Binomial(TNodesManager& _manager)
      : manager(_manager), head(nullptr), size(0), top(nullptr) {}

  constexpr TSelf Make() const { return TSelf(manager); }

  constexpr bool Empty() const { return !head; }

  constexpr unsigned Size() const { return size; }

  constexpr void Add(const TData& v) {
    Node* pv = manager.New();
    pv->value = v;
    AddOneNode(pv);
    ++size;
  }

  constexpr const Node* TopNode() const {
    assert(!Empty());
    if (!top) SetTopNode();
    return top;
  }

  constexpr const TData& Top() const { return TopNode()->GetValue(); }

  constexpr void Pop() { DeleteI(TopNode()); }

  constexpr TData Extract() {
    const TData v = Top();
    Pop();
    return v;
  }

  constexpr void Union(TSelf& h) {
    assert(&manager == &(h.manager));
    Union(h.head);
    h.head = nullptr;
    size += h.size;
    h.size = 0;
    h.ResetTopNode();
  }

 protected:
  TCompare compare;
  TNodesManager& manager;
  Node* head;
  unsigned size;
  mutable Node* top;

 protected:
  constexpr bool Compare(Node* l, Node* r) const {
    return compare(l->GetValue(), r->GetValue());
  }

  constexpr void ResetTopNode() const { top = nullptr; }

  constexpr void SetTopNode() const {
    top = head;
    for (Node* c = top->s; c; c = c->s) {
      if (Compare(c, top)) top = c;
    }
  }

  constexpr Node* TopNode() {
    assert(!Empty());
    if (!top) SetTopNode();
    return top;
  }

  static constexpr void Link(Node* x, Node* y) {
    x->s = y->l;
    y->l = x;
    ++y->d;
  }

  constexpr void AddOneNode(Node* p) {
    ResetTopNode();
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

  static constexpr Node* Merge(Node* h1, Node* h2) {
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

  constexpr void Compress() {
    if (Empty()) return;
    ResetTopNode();
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
          if (pp) {
            pp->s = pn;
          } else {
            head = pn;
          }
          Link(pc, pn);
          pc = pn;
        }
      }
    }
  }

  constexpr void Union(Node* rhead) {
    head = Merge(head, rhead);
    Compress();
  }

  constexpr void CutTree(Node* node) {
    if (head == node) {
      head = node->s;
    } else {
      Node* c = head;
      for (; c->s != node;) c = c->s;
      c->s = node->s;
    }
    node->s = nullptr;
  }

  constexpr Node* CutChildren(Node* node) {
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

  constexpr void DeleteI(Node* node) {
    assert(node);
    ResetTopNode();
    CutTree(node);
    Union(CutChildren(node));
    manager.Release(node);
    --size;
  }
};
}  // namespace base
}  // namespace heap
