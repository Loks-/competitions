#pragma once

#include "common/binary_search_tree/base/left.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/traversal.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/tree/euler_tour_trees_splay_tree.h"
#include "common/template.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

// Work in progress.
namespace graph {
namespace dynamic {
namespace connectivity {
// Keep spannig tree based on EulerTourTreesSplayTree.
class SpanningTreeETTST {
 public:
  using TGraph = graph::dynamic::UndirectedGraph<Empty>;
  using TEdge = typename TGraph::TEdge;
  using TEdgeID = TEdge*;
  using TNode = typename graph::EulerTourTreesSplayTree::TNode;

 protected:
  TGraph g;
  graph::EulerTourTreesSplayTree ett;
  unsigned ncomponents;
  std::unordered_set<TEdge*> ett_edges;

 public:
  SpanningTreeETTST(unsigned size) : g(size), ett(size), ncomponents(size) {}

  void Print(unsigned index) {
    auto v = bst::base::Traverse<TNode, unsigned>(
        bst::base::root(ett.Node(index, false)),
        bst::base::ETraversalOrder::Inorder);
    for (auto x : v) std::cout << x << " ";
    std::cout << std::endl;
  }

  TEdgeID InsertEdge(unsigned from, unsigned to) {
    std::cout << "Insert Edge " << from << "\t" << to << std::endl;
    auto e = g.AddEdge(from, to);
    if (!ett.SameTree(from, to)) {
      std::cout << "\tNew edge for ETT " << from << "\t" << to << std::endl;
      --ncomponents;
      ett.InsertEdge(from, to);
      ett_edges.insert(e);
      Print(from);
    }
    return e;
  }

  void RemoveEdge(TEdgeID edge) {
    std::cout << "Remove Edge " << edge->from << "\t" << edge->to << std::endl;
    g.DeleteEdge(edge);
    auto it = ett_edges.find(edge);
    if (it != ett_edges.end()) {
      std::cout << "\tEdge was used in ETT" << std::endl;
      ett_edges.erase(it);
      auto u1 = edge->from, u2 = edge->to;
      ett.RemoveEdge(u1, u2);
      if (ett.TreeSize(u1) > ett.TreeSize(u2)) std::swap(u1, u2);
      Print(u1);
      Print(u2);
      auto node = ett.Node(u1, false);
      node = bst::base::left(bst::base::root(node));
      bool found = false;
      for (; !found && node; node = bst::base::left(node->right)) {
        ett.Splay(node);
        unsigned raw_index = ett.RawIndex(node);
        if (raw_index & 1) continue;
        unsigned u = raw_index / 2;
        for (auto& e : g.Edges(u)) {
          auto nto = ett.Node(e->to, false);
          ett.Splay(nto);
          if (!node->p) {
            // e point to different tree
            std::cout << "\tNew edge for ETT " << u << "\t" << e->to
                      << std::endl;
            found = true;
            ett.InsertEdge(u, e->to);
            ett_edges.insert(e);
            Print(u);
            break;
          } else {
            ett.Splay(node);
          }
        }
      }
      if (!found) ++ncomponents;
    }
  }

  unsigned Components() const { return ncomponents; }
};
}  // namespace connectivity
}  // namespace dynamic
}  // namespace graph
