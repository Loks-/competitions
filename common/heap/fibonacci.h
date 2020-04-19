#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace heap {
template <class TTData, class TTCompare = std::less<TTData>,
          template <class TNode> class TTNodesManager = NodesManager>
class Fibonacci {
 public:
  static const unsigned mask = (1u << 31);
  using TData = TTData;
  using TCompare = TTCompare;
  using TSelf = Fibonacci<TData, TCompare, TTNodesManager>;

  class Node : public BaseNode {
   public:
    TData value;
    Node *p, *l, *r, *c;
    unsigned d;

    Node() { Clear(); }
    Node(const TData& _value) : value(_value) { Clear(); }

    void Clear() {
      p = l = r = c = nullptr;
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
  std::vector<Node*> vconsolidate;

  bool Compare(Node* l, Node* r) const { return compare(l->value, r->value); }

 public:
  explicit Fibonacci(TNodesManager& _nodes_manager)
      : nodes_manager(_nodes_manager), head(nullptr), size(0) {}

  TSelf Make() const { return TSelf(nodes_manager); }
  bool Empty() const { return !head; }
  unsigned Size() const { return size; }

  void AddNode(Node* node) {
    if (!head) {
      node->l = node->r = node;
      head = node;
    } else {
      node->r = head->r;
      node->r->l = node;
      head->r = node;
      node->l = head;
      if (Compare(node, head)) head = node;
    }
    ++size;
  }

  Node* Add(const TData& v) {
    Node* p = nodes_manager.New();
    p->value = v;
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
    if (Empty()) {
      head = h.head;
    } else {
      Union(h.head);
      if (Compare(h.head, head)) head = h.head;
    }
    h.head = nullptr;
    size += h.size;
    h.size = 0;
  }

  void DecreaseValue(Node* node, const TData& new_value) {
    assert(node);
    node->value = new_value;
    if (node->p) {
      if (Compare(node, node->p)) {
        CutNode(node);
        if (Compare(node, head)) head = node;
      }
    } else {
      if ((node != head) && Compare(node, head)) head = node;
    }
  }

  void DecreaseValueIfLess(Node* node, const TData& new_value) {
    assert(node);
    if (compare(new_value, node->value)) DecreaseValue(node, new_value);
  }

  void IncreaseValue(Node* node, const TData& new_value) {
    assert(node);
    node->value = new_value;
    if (node->c) {
      ClearParentLink(node->c);
      Union(node->c);
      node->c = nullptr;
      Node* p = node->p;
      if (p && Marked(p) && p->p)
        CutNode(p);
      else
        Mark(p);
    }
    if (node == head) {
      Consolidate();
    }
  }

  void SetValue(Node* node, const TData& new_value) {
    assert(node);
    if (compare(node->value, new_value))
      IncreaseValue(node, new_value);
    else
      DecreaseValue(node, new_value);
  }

  void RemoveNode(Node* node) {
    if (node->c) {
      ClearParentLink(node->c);
      Union(node->c);
      node->c = nullptr;
    }
    RemoveFromList(node);
    if (node == head) {
      if (node != node->r) {
        head = node->r;
        Consolidate();
      } else {
        head = nullptr;
      }
    } else if (node->p) {
      Node* p = node->p;
      if (node == p->c) {
        if (node->r == node)
          p->c = nullptr;
        else
          p->c = node->r;
      }
      node->p = nullptr;
      if (Marked(p) && p->p)
        CutNode(p);
      else
        Mark(p);
    }
    --size;
  }

  void Delete(Node* node) {
    RemoveNode(node);
    nodes_manager.Release(node);
  }

 protected:
  static unsigned Degree(const Node* node) { return node->d & ~mask; }
  static void Mark(Node* node) { node->d |= mask; }
  static unsigned Marked(const Node* node) { return node->d & mask; }

  Node* TopNode() {
    assert(head);
    return head;
  }

  void Union(Node* node) {
    if (!node) return;
    node->l->r = head->r;
    head->r->l = node->l;
    head->r = node;
    node->l = head;
  }

  static void ClearParentLink(Node* node) {
    node->p = nullptr;
    for (Node* c = node->r; c != node; c = c->r) c->p = nullptr;
  }

  static void RemoveFromList(Node* node) {
    node->r->l = node->l;
    node->l->r = node->r;
  }

  static void Link(Node* x, Node* y) {
    if (y->c) {
      x->r = y->c->r;
      y->c->r = x;
      x->r->l = x;
      x->l = y->c;
    } else {
      y->c = x;
      x->l = x->r = x;
    }
    x->p = y;
    ++y->d;
    x->d &= ~mask;
  }

  void VCAdjust(unsigned d) {
    if (vconsolidate.size() <= d) vconsolidate.resize(d + 1);
  }

  Node* VCGet(unsigned d) {
    VCAdjust(d);
    return vconsolidate[d];
  }

  void Consolidate() {
    std::fill(vconsolidate.begin(), vconsolidate.end(), nullptr);
    unsigned d = Degree(head);
    VCAdjust(d);
    vconsolidate[d] = head;
    for (Node *p = head->r, *q, *pn; p != head; p = pn) {
      pn = p->r;
      d = Degree(p);
      for (; (q = VCGet(d)); ++d) {
        if (Compare(p, q)) {
          Link(q, p);
        } else {
          Link(p, q);
          p = q;
        }
        vconsolidate[d] = nullptr;
      }
      vconsolidate[d] = p;
    }
    head = nullptr;
    for (Node* p : vconsolidate) {
      if (!p) continue;
      if (head) {
        p->r = head->r;
        head->r = p;
        p->r->l = p;
        p->l = head;
        if (Compare(p, head)) head = p;
      } else {
        head = p;
        p->r = p->l = p;
      }
    }
  }

  void CutNode(Node* node) {
    for (;;) {
      Node *p = node->p, *r = node->r;
      if (r != node) {
        if (p->c == node) p->c = r;
        r->l = node->l;
        r->l->r = r;
      } else {
        p->c = nullptr;
      }
      node->p = nullptr;
      node->r = head->r;
      node->r->l = node;
      node->l = head;
      head->r = node;
      if (Marked(p) && p->p) {
        node = p;
      } else {
        Mark(p);
        break;
      }
    }
  }
};
}  // namespace heap
