#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include "common/template.h"
#include <functional>

namespace heap {
template <class TTData, class TTCompare = std::less<TTData>,
          template <class TNode> class TTNodesManager = NodesManager,
          bool _multipass = false, bool _auxiliary = false>
class Pairing {
 public:
  static const bool multipass = _multipass;
  static const bool auxiliary = _auxiliary;
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf = Pairing<TData, TCompare, TTNodesManager, multipass, auxiliary>;

  class Node : public BaseNode {
   public:
    TData value;
    Node *p, *l, *r;

    Node() { Clear(); }
    Node(const TData& _value) : value(_value) { Clear(); }

    void Clear() { p = l = r = nullptr; }
  };

  using TNodesManager = TTNodesManager<Node>;

 protected:
  TCompare compare;
  TNodesManager& nodes_manager;
  Node* head;
  unsigned size;

  bool Compare(Node* l, Node* r) const { return compare(l->value, r->value); }

 public:
  Pairing(TNodesManager& _nodes_manager)
      : nodes_manager(_nodes_manager), head(nullptr), size(0) {}

  TSelf Make() const { return TSelf(nodes_manager); }
  bool Empty() const { return !head; }
  unsigned Size() const { return size; }

  void AddNode(Node* node) { AddNode(node, TFakeBool<auxiliary>()); }

  Node* Add(const TData& v) {
    Node* p = nodes_manager.New(v);
    AddNode(p);
    return p;
  }

  const TData& Top() { return TopNode()->value; }

  void Pop() {
    ProcessAux();
    Delete(head);
  }

  TData Extract() {
    TData v = Top();
    Pop();
    return v;
  }

  void Union(TSelf& h) {
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

  void DecreaseValue(Node* node, const TData& new_value) {
    assert(node);
    node->value = new_value;
    if (node != head) {
      assert(node->p);
      if (node == node->p->l)
        node->p->l = node->r;
      else
        node->p->r = node->r;
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

  void DecreaseValueIfLess(Node* node, const TData& new_value) {
    assert(node);
    if (compare(new_value, node->value)) DecreaseValue(node, new_value);
  }

  void IncreaseValue(Node* node, const TData& new_value) {
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

  void SetValue(Node* node, const TData& new_value) {
    assert(node);
    if (compare(node->value, new_value))
      IncreaseValue(node, new_value);
    else
      DecreaseValue(node, new_value);
  }

  void Delete(Node* node) {
    RemoveNode(node);
    nodes_manager.Release(node);
  }

 protected:
  static Node* Link(Node* x, Node* y) {
    x->r = y->l;
    if (x->r) x->r->p = x;
    y->l = x;
    x->p = y;
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
    if (f) f->p = nullptr;
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
    if (f) f->p = nullptr;
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
    assert(head);
    ProcessAux();
    return head;
  }

  void RemoveNode(Node* x) {
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
}  // namespace heap
