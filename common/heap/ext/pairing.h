#pragma once

#include "common/base.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
#include "common/template.h"

#include <functional>

namespace heap {
namespace ext {
// Memory  -- O(N)
// Add     -- O(1)
// DecV    -- o(log N)
// IncV    -- O(log N)
// Top     -- O(1)
// Pop     -- O(log N) amortized
// Union   -- O(1)
template <class TTData, class TTCompare = std::less<TTData>,
          template <class TNode> class TTNodesManager = memory::NodesManager,
          bool multipass = false, bool auxiliary = false>
class Pairing {
 public:
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf = Pairing<TData, TCompare, TTNodesManager, multipass, auxiliary>;

  class Node : public memory::Node {
   public:
    TData value;
    Node *p, *l, *r;

   public:
    constexpr Node() { Clear(); }

    constexpr Node(const TData& _value) : value(_value) { Clear(); }

    constexpr void Clear() { p = l = r = nullptr; }
  };

  using TNodesManager = TTNodesManager<Node>;

 protected:
  TCompare compare;
  TNodesManager& nodes_manager;
  Node* head;
  unsigned size;

  constexpr bool Compare(Node* l, Node* r) const {
    return compare(l->value, r->value);
  }

 public:
  constexpr explicit Pairing(TNodesManager& _nodes_manager)
      : nodes_manager(_nodes_manager), head(nullptr), size(0) {}

  constexpr TSelf Make() const { return TSelf(nodes_manager); }

  constexpr bool Empty() const { return !head; }

  constexpr unsigned Size() const { return size; }

  constexpr void AddNode(Node* node) { AddNode(node, TFakeBool<auxiliary>()); }

  constexpr Node* Add(const TData& v) {
    Node* p = nodes_manager.New();
    p->value = v;
    AddNode(p);
    return p;
  }

  constexpr const TData& Top() { return TopNode()->value; }

  constexpr void Pop() {
    ProcessAux();
    Delete(head);
  }

  constexpr TData Extract() {
    const TData v = Top();
    Pop();
    return v;
  }

  constexpr void Union(TSelf& h) {
    assert(&nodes_manager == &(h.nodes_manager));
    if (h.Empty()) return;
    if (Empty()) {
      head = h.head;
    } else {
      ProcessAux();
      h.ProcessAux();
      ComparisonLinkHead(h.head);
    }
    h.head = nullptr;
    size += h.size;
    h.size = 0;
  }

  constexpr void DecreaseValue(Node* node, const TData& new_value) {
    assert(node);
    node->value = new_value;
    if (node != head) {
      assert(node->p);
      if (node == node->p->l)
        node->p->l = node->r;
      else
        node->p->r = node->r;
      if (node->r) node->r->p = node->p;
      if (auxiliary) {
        node->p = head;
        node->r = head->r;
        if (node->r) node->r->p = node;
        head->r = node;
      } else {
        node->p = node->r = nullptr;
        ComparisonLinkHead(node);
      }
    }
  }

  constexpr void DecreaseValueIfLess(Node* node, const TData& new_value) {
    assert(node);
    if (compare(new_value, node->value)) DecreaseValue(node, new_value);
  }

  constexpr void IncreaseValue(Node* node, const TData& new_value) {
    assert(node);
    node->value = new_value;
    if (auxiliary && (node == head)) ProcessAux();
    if (node == head) {
      node->r = node->l;
      node->l = nullptr;
      head = Compress(node);
    } else {
      Node* t = Compress(node->l);
      if (t) {
        t->r = node->r;
        if (t->r) t->r->p = t;
        node->r = t;
        t->p = node;
      }
    }
  }

  constexpr void SetValue(Node* node, const TData& new_value) {
    assert(node);
    if (compare(node->value, new_value))
      IncreaseValue(node, new_value);
    else
      DecreaseValue(node, new_value);
  }

  constexpr void Delete(Node* node) {
    RemoveNode(node);
    nodes_manager.Release(node);
  }

 protected:
  constexpr static Node* Link(Node* x, Node* y) {
    x->r = y->l;
    if (x->r) x->r->p = x;
    y->l = x;
    x->p = y;
    return y;
  }

  constexpr Node* ComparisonLink(Node* x, Node* y) {
    if (Compare(x, y))
      return Link(y, x);
    else
      return Link(x, y);
  }

  constexpr void ComparisonLinkHead(Node* x) {
    if (Compare(x, head)) {
      Link(head, x);
      head = x;
    } else {
      Link(x, head);
    }
  }

  constexpr void AddNode(Node* node, TFakeFalse) {
    if (!head)
      head = node;
    else
      ComparisonLinkHead(node);
    ++size;
  }

  constexpr void AddNode(Node* node, TFakeTrue) {
    if (!head) {
      head = node;
    } else {
      node->r = head->r;
      if (node->r) node->r->p = node;
      head->r = node;
      node->p = head;
    }
    ++size;
  }

  constexpr Node* Compress(Node* f, TFakeFalse) {
    if (f && f->r) {
      Node* l = nullptr;
      for (; f && f->r;) {
        Node *fr = f->r, *frr = fr->r;
        Node* t = ComparisonLink(f, fr);
        t->r = l;
        f = frr;
        l = t;
      }
      if (f)
        f->r = l;
      else
        f = l;
      for (l = f->r; l;) {
        Node* lr = l->r;
        f = ComparisonLink(f, l);
        l = lr;
      }
      f->r = nullptr;
    }
    if (f) f->p = nullptr;
    return f;
  }

  constexpr Node* Compress(Node* f, TFakeTrue) {
    if (f && f->r) {
      Node* l = f->r;
      for (; l->r;) l = l->r;
      for (; f->r != l; l = l->r) {
        Node *fr = f->r, *frr = fr->r;
        l->r = ComparisonLink(f, fr);
        f = frr;
      }
      f = ComparisonLink(f, l);
      f->r = nullptr;
    }
    if (f) f->p = nullptr;
    return f;
  }

  constexpr Node* Compress(Node* f) {
    return Compress(f, TFakeBool<multipass>());
  }

  constexpr void ProcessAux(TFakeFalse) {}

  constexpr void ProcessAux(TFakeTrue) {
    if (head->r) {
      Node* t = Compress(head->r, TFakeTrue());
      head->r = nullptr;
      ComparisonLinkHead(t);
    }
  }

  constexpr void ProcessAux() { ProcessAux(TFakeBool<auxiliary>()); }

  constexpr Node* TopNode() {
    assert(head);
    ProcessAux();
    return head;
  }

  constexpr void RemoveNode(Node* x) {
    if (x == head) {
      Node* t = Compress(x->l);
      x->l = nullptr;
      head = t;
    } else {
      Node* t = Compress(x->l);
      if (t) {
        if (x->p->l == x)
          x->p->l = t;
        else
          x->p->r = t;
        t->p = x->p;
        t->r = x->r;
        if (x->r) x->r->p = t;
      } else {
        if (x->p->l == x)
          x->p->l = x->r;
        else
          x->p->r = x->r;
        if (x->r) x->r->p = x->p;
      }
      x->p = x->r = nullptr;
    }
    --size;
  }
};
}  // namespace ext
}  // namespace heap
