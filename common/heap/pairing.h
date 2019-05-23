#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include "common/template.h"
#include <functional>

namespace heap {
template <class TTData, class TTCompare = std::less<TTData>,
          template <class TNode> class TTNodesManager = NodesManager,
          bool _multipass = true>
class Pairing {
 public:
  static const bool multipass = _multipass;
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf = Pairing<TData, TCompare, TTNodesManager, multipass>;

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

  Node* TopNode() {
    assert(head);
    return head;
  }

 public:
  Pairing(TNodesManager& _nodes_manager)
      : nodes_manager(_nodes_manager), head(nullptr), size(0) {}

  TSelf Make() const { return TSelf(nodes_manager); }
  bool Empty() const { return !head; }
  unsigned Size() const { return size; }

  void AddNode(Node* node) {
    if (!head)
      head = node;
    else
      ComparisonLinkHead(node);
    ++size;
  }

  Node* Add(const TData& v) {
    Node* p = nodes_manager.New(v);
    AddNode(p);
    return p;
  }

  const Node* TopNode() const {
    assert(head);
    return head;
  }

  const TData& Top() const { return TopNode()->value; }

  void Pop() { Delete(head); }

  TData Extract() {
    TData v = Top();
    Pop();
    return v;
  }

  void Union(TSelf& h) {
    assert(&nodes_manager == &(h.nodes_manager));
    if (h.Empty()) return;
    if (Empty())
      head = h.head;
    else
      ComparisonLinkHead(h.head);
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
      node->p = node->r = nullptr;
      ComparisonLinkHead(node);
    }
  }

  void DecreaseValueIfLess(Node* node, const TData& new_value) {
    assert(node);
    if (compare(new_value, node->value)) DecreaseValue(node, new_value);
  }

  //   void IncreaseValue(Node* node, const TData& new_value) {
  //     // assert(node);
  //     // node->value = new_value;
  //     assert(false);
  //   }

  //   void SetValue(Node* node, const TData& new_value) {
  //     assert(node);
  //     if (compare(node->value, new_value))
  //       IncreaseValue(node, new_value);
  //     else
  //       DecreaseValue(node, new_value);
  //   }

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

  void RemoveNonHead(Node* x) {
    // ...
    assert(false);
  }

  void RemoveHead(TFakeFalse) {
    Node* f = head->l;
    head->l = nullptr;
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
    head = f;
  }

  void RemoveHead(TFakeTrue) {
    Node* f = head->l;
    head->l = nullptr;
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
    head = f;
  }

  void RemoveNode(Node* x) {
    if (x == head)
      RemoveHead(TFakeBool<multipass>());
    else
      RemoveNonHead(x);
    --size;
  }
};
}  // namespace heap
