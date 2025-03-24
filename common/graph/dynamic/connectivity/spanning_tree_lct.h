#pragma once

#include "common/base.h"
#include "common/data_structures/unsigned_set.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/tree/link_cut_tree.h"

#include <algorithm>
#include <stack>

namespace graph {
namespace dynamic {
namespace connectivity {
// Keep spannig tree based on LinkCutTree.
class SpanningTreeLCT {
 public:
  using TGraph = graph::dynamic::UndirectedGraph<bool>;
  using TEdge = typename TGraph::TEdge;
  using TEdgeID = TEdge*;
  using TLCT = graph::LinkCutTree<MetaEmpty, std::tuple<>>;
  using TNode = typename TLCT::TNode;

 protected:
  TGraph g;
  TLCT lct;
  unsigned ncomponents;
  ds::UnsignedSet uset;
  std::stack<unsigned> s;

 public:
  SpanningTreeLCT(unsigned size)
      : g(size), lct(size), ncomponents(size), uset(size) {}

  TNode* Node(unsigned index) { return lct.Node(index); }

  bool SameTree(TNode* node1, TNode* node2) {
    return lct.FindRoot(node1) == lct.FindRoot(node2);
  }

  TEdgeID InsertEdge(unsigned u1, unsigned u2) {
    auto e = g.AddEdge(u1, u2, false);
    auto n1 = Node(u1), n2 = Node(u2);
    if (!SameTree(n1, n2)) {
      --ncomponents;
      lct.SetRoot(n1);
      lct.Link(n1, n2);
      e->data = true;
    }
    return e;
  }

  void RemoveEdge(TEdgeID edge) {
    if (edge->data) {
      const auto u1 = edge->u1, u2 = edge->u2;
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
          const auto v = e->Other(u);
          if (uset.HasKey(v)) continue;
          const auto r = lct.FindRoot(Node(v));
          if (r != n1) {
            assert(r == n2);
            found = true;
            auto nu = Node(u);
            lct.SetRoot(nu);
            lct.Link(nu, Node(v));
            e->data = true;
            break;
          } else {
            uset.Insert(v);
            s.push(v);
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
