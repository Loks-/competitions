#pragma once

#include "common/binary_search_tree/splay_tree.h"

#include <algorithm>

// Work in progress.
namespace graph {
// Euler Tour Trees based on SplayTree
class EulerTourTreesSplayTree {
 public:
  using TBSTree = bst::SplayTree<false, unsigned>;
  using TNode = typename TBSTree::TNode;

 protected:
  TBSTree bstree;

 public:
  explicit EulerTourTreesSplayTree(unsigned size) : bstree(2 * size) {
    for (unsigned i = 0; i < size; ++i) {
      auto node_l = bstree.New(i);
      auto node_r = bstree.New(i);
      node_l->SetR(node_r);
      node_l->UpdateInfo();
    }
  }

  unsigned RawIndex(TNode* node) const {
    return bstree.RawIndex(node);
  }

  unsigned Index(TNode* node) const {
    return node->data;
    // return bstree.RawIndex(node) / 2;
  }

  TNode* Node(unsigned index, bool right) {
    return bstree.NodeByRawIndex(2 * index + (right ? 1 : 0));
  }

  void Splay(TNode* node) { bstree.Splay(node); }
  unsigned Order(TNode* node) { return bstree.Order(node); }

  TNode* FindRoot(TNode* node) {
    Splay(node);
    for (; node->l;) node = node->l;
    Splay(node);
    return node;
  }

  unsigned FindRoot(unsigned index) {
    return Index(FindRoot(Node(index, false)));
  }

  void SetRoot(unsigned index) {
    auto nl = Node(index, false), nr = Node(index, true);
    if (Order(nr) < Order(nl)) std::swap(nl, nr);
    auto l = bstree.SplitL(nl);
    auto r = bstree.SplitR(nr);
    auto m = bstree.SplitL(nr);
    bstree.Join(bstree.Join(m, r), bstree.Join(l, nr));
  }

  void Cut(unsigned index) {
    auto nl = Node(index, false), nr = Node(index, true);
    if (Order(nr) < Order(nl)) std::swap(nl, nr);
    bstree.Join(bstree.SplitL(nl), bstree.SplitR(nr));
  }

  // Link root to node
  void Link(unsigned index_root, unsigned index_node) {
    auto node_c = Node(index_root, false), node_p0 = Node(index_node, false), node_p1 = Node(index_node, true);
    if (Order(node_p1) < Order(node_p0)) node_p0 = node_p1;
    Splay(node_c);
    auto r = bstree.SplitR(node_p0);
    bstree.Join(bstree.Join(node_p0, node_c), r);
  }

  void InsertEdge(unsigned from, unsigned to) {
    SetRoot(to);
    Link(to, from);
  }

  void RemoveEdge(unsigned from, unsigned to) {
    auto u0 = Order(Node(from, false)), u1 = Order(Node(from, true)), 
         v0 = Order(Node(to, false)), v1 = Order(Node(to, true));
    if (u1 < u0) std::swap(u0, u1);
    if (v1 < v0) std::swap(v0, v1);
    Cut((u1 - u0) < (v1 - v0) ? from : to);
  }

  unsigned TreeSize(unsigned index) {
    auto node = Node(index, false);
    Splay(node);
    return node->info.size;
  }

  bool SameTree(unsigned index1, unsigned index2) {
    auto node1 = Node(index1, false), node2 = Node(index2, false);
    // return FindRoot(node1) == FindRoot(node2);
    Splay(node1);
    Splay(node2);
    return node1->p;
  }
};
}  // namespace graph
