#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/none.h"
#include "common/data_structures/unsigned_set.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/tree/link_cut_tree.h"
#include "common/template.h"
#include "common/vector/enumerate.h"

#include <algorithm>
#include <stack>
#include <unordered_set>

namespace graph {
namespace dynamic {
namespace connectivity {
// Keep spannig tree based on LinkCutTree.
class SpanningTreeLCT {
 public:
  using TGraph = graph::dynamic::Graph<TEmpty>;
  using TEdge = typename TGraph::TEdge;
  using TEdgeID = TEdge*;
  using TLCT = graph::LinkCutTree<unsigned, bst::info::None>;
  using TNode = typename TLCT::TNode;

 protected:
  TGraph g;
  TLCT lct;
  unsigned ncomponents;
  std::unordered_set<TEdge*> lct_edges;
  ds::UnsignedSet uset;
  std::stack<unsigned> s;

 public:
  SpanningTreeLCT(unsigned size)
      : g(size),
        lct(nvector::Enumerate<unsigned>(0, size)),
        ncomponents(size),
        uset(size) {}

  TNode* Node(unsigned index) { return lct.Node(index); }

  bool SameTree(TNode* node0, TNode* node1) {
    return lct.FindRoot(node0) == lct.FindRoot(node1);
  }

  TEdgeID InsertEdge(unsigned from, unsigned to) {
    auto e = g.AddEdge(from, to);
    auto n1 = Node(from), n2 = Node(to);
    if (!SameTree(n1, n2)) {
      --ncomponents;
      lct.SetRoot(n1);
      lct.Link(n1, n2);
      lct_edges.insert(e);
      lct_edges.insert(e->reversed_edge);
    }
    return e;
  }

  void RemoveEdge(TEdgeID edge) {
    auto it = lct_edges.find(edge);
    if (it != lct_edges.end()) {
      // std::cout << "\tEdge was used in ETT" << std::endl;
      lct_edges.erase(it);
      lct_edges.erase(edge->reversed_edge);
      auto u1 = edge->from, u2 = edge->to;
      auto n1 = Node(u1), n2 = Node(u2);
      g.DeleteEdge(edge);
      lct.SetRoot(n1);
      lct.Cut(n2);
      assert(lct.FindRoot(n2) == n2);
      bool found = false;
      uset.Clear();
      uset.Insert(u1);
      for (; !s.empty();) s.pop();
      for (s.push(u1); !found && !s.empty();) {
        unsigned u = s.top();
        s.pop();
        for (auto& e : g.Edges(u)) {
          if (uset.HasKey(e->to)) continue;
          auto r = lct.FindRoot(Node(e->to));
          if (r != n1) {
            assert(r == n2);
            found = true;
            auto nu = Node(u);
            lct.SetRoot(nu);
            lct.Link(nu, Node(e->to));
            lct_edges.insert(e);
            lct_edges.insert(e->reversed_edge);
            break;
          } else {
            uset.Insert(e->to);
            s.push(e->to);
          }
        }
      }
      if (!found) ++ncomponents;
    } else {
      g.DeleteEdge(edge);
    }
  }

  unsigned Components() const { return ncomponents; }
};
}  // namespace connectivity
}  // namespace dynamic
}  // namespace graph
