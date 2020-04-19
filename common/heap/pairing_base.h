#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include "common/template.h"

#include <functional>

namespace heap {
// Simplified Pairing heap.
template <class TTData, class TTCompare = std::less<TTData>,
          template <class TNode> class TTNodesManager = NodesManager,
          bool _multipass = false, bool _auxiliary = false>
class PairingBase {
 public:
  static const bool multipass = _multipass;
  static const bool auxiliary = _auxiliary;
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf =
      PairingBase<TData, TCompare, TTNodesManager, multipass, auxiliary>;

  class Node : public BaseNode {
   public:
    TData value;
    Node *l, *r;

    Node() { Clear(); }
    Node(const TData& _value) : value(_value) { Clear(); }

    void Clear() { l = r = nullptr; }
  };

  using TNodesManager = TTNodesManager<Node>;

 protected:
  TCompare compare;
  TNodesManager nodes_manager;
  Node* head;
  unsigned size;

  bool Compare(Node* l, Node* r) const { return compare(l->value, r->value); }

 public:
  explicit PairingBase(unsigned expected_size)
      : nodes_manager(expected_size), head(nullptr), size(0) {}

  bool Empty() const { return !head; }
  unsigned Size() const { return size; }

  void Add(const TData& v) {
    Node* p = nodes_manager.New();
    p->value = v;
    AddNode(p);
  }

  const TData& Top() { return TopNode()->value; }

  void Pop() {
    assert(head);
    ProcessAux();
    Node* t = head;
    head = Compress(head->l);
    t->l = nullptr;
    --size;
    nodes_manager.Release(t);
  }

  TData Extract() {
    TData v = Top();
    Pop();
    return v;
  }

 protected:
  static Node* Link(Node* x, Node* y) {
    x->r = y->l;
    y->l = x;
    return y;
  }

  Node* ComparisonLink(Node* x, Node* y) {
    if (Compare(x, y))
      return Link(y, x);
    else
      return Link(x, y);
  }

  void ComparisonLinkHead(Node* x) {
    if (Compare(x, head)) {
      Link(head, x);
      head = x;
    } else {
      Link(x, head);
    }
  }

  void AddNode(Node* node, TFakeFalse) {
    if (!head)
      head = node;
    else
      ComparisonLinkHead(node);
    ++size;
  }

  void AddNode(Node* node, TFakeTrue) {
    if (!head) {
      head = node;
    } else {
      node->r = head->r;
      head->r = node;
    }
    ++size;
  }

  void AddNode(Node* node) { AddNode(node, TFakeBool<auxiliary>()); }

  Node* Compress(Node* f, TFakeFalse) {
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
    return f;
  }

  Node* Compress(Node* f, TFakeTrue) {
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

  Node* Compress(Node* f) { return Compress(f, TFakeBool<multipass>()); }

  void ProcessAux(TFakeFalse) {}

  void ProcessAux(TFakeTrue) {
    if (head->r) {
      Node* t = Compress(head->r, TFakeTrue());
      head->r = nullptr;
      ComparisonLinkHead(t);
    }
  }

  void ProcessAux() { ProcessAux(TFakeBool<auxiliary>()); }

  Node* TopNode() {
    ProcessAux();
    return head;
  }
};
}  // namespace heap
