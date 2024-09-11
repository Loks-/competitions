#pragma once

#include "common/base.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
#include "common/template.h"

#include <functional>

namespace heap {
namespace base {
// Simplified Pairing heap.
// Memory  -- O(N)
// Add     -- O(1)
// Top     -- O(1)
// Pop     -- O(log N) amortized
// Union   -- O(1)
template <class TData, class TCompare = std::less<TData>,
          template <class TNode> class TTNodesManager = memory::NodesManager,
          bool multipass = false, bool auxiliary = false>
class Pairing {
 public:
  class Node : public memory::Node {
   public:
    TData value;
    Node *l, *r;

   public:
    constexpr Node() { Clear(); }

    constexpr Node(const TData& _value) : value(_value) { Clear(); }

    constexpr void Clear() { l = r = nullptr; }

    constexpr const TData& GetValue() const { return value; }
  };

 protected:
  TCompare compare;
  TTNodesManager<Node> manager;
  Node* head;
  unsigned size;

 protected:
  constexpr bool Compare(Node* l, Node* r) const {
    return compare(l->GetValue(), r->GetValue());
  }

 public:
  constexpr explicit Pairing(unsigned expected_size)
      : manager(expected_size), head(nullptr), size(0) {}

  constexpr explicit Pairing(const std::vector<TData>& v) : Pairing(v.size()) {
    for (auto& u : v) Add(u);
  }

  constexpr bool Empty() const { return !head; }

  constexpr unsigned Size() const { return size; }

  constexpr void Add(const TData& v) {
    Node* p = manager.New();
    p->value = v;
    AddNode(p);
  }

  constexpr const TData& Top() { return TopNode()->GetValue(); }

  constexpr void Pop() {
    assert(head);
    ProcessAux();
    Node* old_head = head;
    head = Compress(head->l);
    old_head->l = nullptr;
    --size;
    manager.Release(old_head);
  }

  constexpr TData Extract() {
    const TData v = Top();
    Pop();
    return v;
  }

 protected:
  constexpr static Node* Link(Node* x, Node* y) {
    x->r = y->l;
    y->l = x;
    return y;
  }

  constexpr Node* ComparisonLink(Node* x, Node* y) {
    if (Compare(x, y)) {
      return Link(y, x);
    } else {
      return Link(x, y);
    }
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
    if (!head) {
      head = node;
    } else {
      ComparisonLinkHead(node);
    }
    ++size;
  }

  constexpr void AddNode(Node* node, TFakeTrue) {
    if (!head) {
      head = node;
    } else {
      node->r = head->r;
      head->r = node;
    }
    ++size;
  }

  constexpr void AddNode(Node* node) { AddNode(node, TFakeBool<auxiliary>()); }

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
      if (f) {
        f->r = l;
      } else {
        f = l;
      }
      for (l = f->r; l;) {
        Node* lr = l->r;
        f = ComparisonLink(f, l);
        l = lr;
      }
      f->r = nullptr;
    }
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
    ProcessAux();
    return head;
  }
};
}  // namespace base
}  // namespace heap
