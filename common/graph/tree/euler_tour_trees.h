#pragma once

#include "common/binary_search_tree/splay_tree.h"

namespace graph {
// Use splay trees for bst operations
class EulerTourTrees {
 public:
  using TBSTree = bst::SplayTree<false, unsigned, bst::info::None>;
  using TNode = typename TBSTree::TNode;

 protected:
  TBSTree bstree;

 public:
  explicit EulerTourTrees(unsigned size) : bstree(2 * size) {
    for (unsigned i = 0; i < size; ++i) {
      auto node_l = bstree.New(i);
      auto node_r = bstree.New(i);
      node_l->SetR(node_r);
      node_l->UpdateInfo();
    }
  }

  unsigned Index(TNode* node) const {
    return node->data;
    // return bstree.RawIndex(node) / 2;
  }

  TNode* Node(unsigned index, bool right) {
    return bstree.NodeByRawIndex(2 * index + (right ? 1 : 0));
  }

  TNode* FindRoot(TNode* node) {
    bstree.Splay(node);
    for (; node->l;) node = node->l;
    bstree.Splay(node);
    return node;
  }

  unsigned FindRoot(unsigned index) {
    return Index(FindRoot(Node(index, false)));
  }

  void Cut(unsigned index) {
    bstree.Join(bstree.SplitL(Node(index, false)),
                bstree.SplitR(Node(index, true)));
  }

  // Link root to node
  void Link(unsigned index_root, unsigned index_node) {
    auto node_c = Node(index_root, false), node_p = Node(index_node, false);
    bstree.Splay(node_c);
    auto r = bstree.SplitR(node_p);
    bstree.Join(bstree.Join(node_p, node_c), r);
  }

  bool SameTree(unsigned index1, unsigned index2) {
    auto node1 = Node(index1, false), node2 = Node(index2, false);
    // return FindRoot(node1) == FindRoot(node2);
    bstree.Splay(node1);
    bstree.Splay(node2);
    return node1->p;
  }
};
}  // namespace graph
