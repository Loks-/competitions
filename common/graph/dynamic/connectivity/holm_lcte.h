#pragma once

#include "common/base.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/tree/lcte/action/none.h"
#include "common/graph/tree/lcte/info/max.h"
#include "common/graph/tree/lcte/info/size.h"
#include "common/graph/tree/lcte/lcte.h"
#include "common/numeric/bits/ulog2.h"
#include "common/template.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace graph {
namespace dynamic {
namespace connectivity {
// Holm algorithm with LCTE for layers representation.
// Memory     -- O(N log N)
// Insert     -- O(log^2 N) amortized
// Delete     -- O(log^2 N) amortized
// SameTree   -- O(log N)
// Components -- O(1)
class HolmLCTE {
 public:
  class EdgeData {
   public:
    unsigned level;
    unsigned index1;
    unsigned index2;

    EdgeData() : level(0) {}
  };

  using TGraph = graph::dynamic::UndirectedGraph<EdgeData>;
  using TEdge = typename TGraph::TEdge;
  using TEdgeID = TEdge*;

  class LEdges {
   public:
    std::vector<TEdgeID> tree_edges;
    std::vector<TEdgeID> graph_edges;
  };

  using TLCTE = graph::lcte::LCTE<
      unsigned, graph::lcte::info::PMax<unsigned, graph::lcte::info::PSize>,
      graph::lcte::info::VMax<unsigned, graph::lcte::info::VSize>,
      graph::lcte::action::None>;
  using TNode = typename TLCTE::TPNode;

 protected:
  unsigned size;
  unsigned L;
  unsigned lsize;
  TGraph g;
  TLCTE lcte;
  std::vector<LEdges> vledges;
  unsigned ncomponents;

 protected:
  void LCTEUpdateInfo(unsigned lu, TNode* node) {
    const auto& ledges = vledges[lu];
    node->data = (!ledges.tree_edges.empty()    ? 2 * lsize + lu
                  : !ledges.graph_edges.empty() ? lsize + lu
                                                : 0u);
    lcte.Access(node);
  }

  void LCTEAddTreeEdge(TEdgeID edge, unsigned lu1, TNode* node1) {
    auto& ledges = vledges[lu1];
    edge->data.index1 = ledges.tree_edges.size();
    ledges.tree_edges.push_back(edge);
    if (ledges.tree_edges.size() == 1) LCTEUpdateInfo(lu1, node1);
  }

  void LCTEAddGraphEdge(TEdgeID edge, unsigned lu1, TNode* node1, bool b12) {
    auto& ledges = vledges[lu1];
    (b12 ? edge->data.index1 : edge->data.index2) = ledges.graph_edges.size();
    ledges.graph_edges.push_back(edge);
    if (ledges.tree_edges.empty() && (ledges.graph_edges.size() == 1))
      LCTEUpdateInfo(lu1, node1);
  }

  void LCTEAddTreeEdge(TEdgeID edge) {
    const unsigned l = edge->data.level / 2;
    const unsigned lu1 = LIndex(edge->u1, l);
    LCTEAddTreeEdge(edge, lu1, Node(lu1));
  }

  void LCTEAddGraphEdge(TEdgeID edge) {
    const unsigned l = edge->data.level / 2;
    const unsigned lu1 = LIndex(edge->u1, l), lu2 = LIndex(edge->u2, l);
    LCTEAddGraphEdge(edge, lu1, Node(lu1), true);
    LCTEAddGraphEdge(edge, lu2, Node(lu2), false);
  }

  void LCTERemoveTreeEdge(TEdgeID edge, unsigned lu1, TNode* node1) {
    auto& ledges = vledges[lu1];
    auto& edges = ledges.tree_edges;
    const auto index = edge->data.index1;
    assert(edges[index] == edge);
    if (index + 1 < edges.size()) {
      edges[index] = edges.back();
      edges[index]->data.index1 = index;
    }
    edges.pop_back();
    if (edges.empty()) LCTEUpdateInfo(lu1, node1);
  }

  void LCTERemoveGraphEdge(TEdgeID edge, unsigned lu1, TNode* node1, bool b12) {
    auto& ledges = vledges[lu1];
    auto& edges = ledges.graph_edges;
    const auto index = (b12 ? edge->data.index1 : edge->data.index2);
    assert(edges[index] == edge);
    if (index + 1 < edges.size()) {
      const auto enew = edges.back();
      edges[index] = enew;
      ((enew->u1 == (lu1 % size)) ? enew->data.index1 : enew->data.index2) =
          index;
    }
    edges.pop_back();
    if (ledges.tree_edges.empty() && edges.empty()) LCTEUpdateInfo(lu1, node1);
  }

  void LCTERemoveTreeEdge(TEdgeID edge) {
    const unsigned l = edge->data.level / 2;
    const unsigned lu1 = LIndex(edge->u1, l);
    LCTERemoveTreeEdge(edge, lu1, Node(lu1));
  }

  void LCTERemoveGraphEdge(TEdgeID edge) {
    const unsigned l = edge->data.level / 2;
    const unsigned lu1 = LIndex(edge->u1, l), lu2 = LIndex(edge->u2, l);
    LCTERemoveGraphEdge(edge, lu1, Node(lu1), true);
    LCTERemoveGraphEdge(edge, lu2, Node(lu2), false);
  }

  void LCTELinkEdge(TNode* node1, TNode* node2) {
    lcte.SetRoot(node1);
    lcte.Link(node1, node2);
  }

  void LCTECutEdge(TNode* node1, TNode* node2) {
    lcte.SetRoot(node1);
    lcte.Cut(node2);
    assert(lcte.FindRoot(node2) == node2);
  }

 public:
  HolmLCTE(unsigned _size)
      : size(_size),
        L(numeric::ULog2(size) + 2),
        lsize(L * size),
        g(size),
        lcte(lsize),
        vledges(lsize),
        ncomponents(size) {}

  unsigned LIndex(unsigned u, unsigned level) const { return u + size * level; }

  TNode* Node(unsigned lindex) { return lcte.Node(lindex); }

  TNode* Node(unsigned u, unsigned level) { return Node(LIndex(u, level)); }

  bool SameTree(TNode* node1, TNode* node2) {
    return lcte.FindRoot(node1) == lcte.FindRoot(node2);
  }

  TEdgeID InsertEdge(unsigned u1, unsigned u2) {
    assert(u1 != u2);
    auto e = g.AddEdge(u1, u2);
    auto n1 = Node(u1), n2 = Node(u2);
    if (SameTree(n1, n2)) {
      LCTEAddGraphEdge(e, u1, n1, true);
      LCTEAddGraphEdge(e, u2, n2, false);
    } else {
      --ncomponents;
      e->data.level = 1;
      LCTELinkEdge(n1, n2);
      LCTEAddTreeEdge(e, u1, n1);
    }
    return e;
  }

  void RemoveEdge(TEdgeID edge) {
    if (edge->data.level & 1) {
      unsigned l = edge->data.level / 2;
      const unsigned u1 = edge->u1, u2 = edge->u2;
      for (unsigned i = 0; i <= l; ++i)
        LCTECutEdge(Node(edge->u1, i), Node(edge->u2, i));
      LCTERemoveTreeEdge(edge);
      g.DeleteEdge(edge);
      ++ncomponents;
      for (++l; l-- > 0;) {
        bool found = false;
        auto node1 = Node(u1, l), node2 = Node(u2, l);
        lcte.Access(node1);
        lcte.Access(node2);
        if (node1->info.tsize > node2->info.tsize) {
          std::swap(node1, node2);
        }
        lcte.SetRoot(node1);
        auto root2 = lcte.FindRoot(node2);
        MetaUse(root2);
        assert(root2 != node1);
        for (; !found && node1->info.tmax; lcte.Access(node1)) {
          const auto value = node1->info.tmax;
          const auto u = value % lsize;
          if (value >= 2 * lsize) {
            // Push up tree edges
            auto& edges = vledges[u].tree_edges;
            for (auto e : edges) {
              e->data.level += 2;
              auto u1 = LIndex(e->u1, l + 1), u2 = LIndex(e->u2, l + 1);
              auto n1 = Node(u1), n2 = Node(u2);
              LCTELinkEdge(n1, n2);
              LCTEAddTreeEdge(e, u1, n1);
            }
            edges.clear();
            LCTEUpdateInfo(u, Node(u));
          } else {
            // Test graph edges
            auto& edges = vledges[u].graph_edges;
            for (unsigned j = edges.size(); j-- > 0;) {
              auto e = edges[j];
              const auto eu1 = LIndex(e->u1, l), eu2 = LIndex(e->u2, l);
              auto n1 = Node(eu1), n2 = Node(eu2);
              auto r1 = lcte.FindRoot(n1), r2 = lcte.FindRoot(n2);
              if (r1 == r2) {
                assert(r1 == node1);
                if (eu1 == u) {
                  LCTERemoveGraphEdge(e, eu2, n2, false);
                } else {
                  LCTERemoveGraphEdge(e, eu1, n1, true);
                }
                e->data.level += 2;
                LCTEAddGraphEdge(e);
              } else {
                assert(r1 - node1 + r2 == root2);
                found = true;
                --ncomponents;
                if (eu1 == u) {
                  LCTERemoveGraphEdge(e, eu2, n2, false);
                } else {
                  LCTERemoveGraphEdge(e, eu1, n1, true);
                }
                edges.resize(j);
                if (j == 0) LCTEUpdateInfo(u, Node(u));
                e->data.level += 1;
                for (unsigned i = 0; i <= l; ++i)
                  LCTELinkEdge(Node(e->u1, i), Node(e->u2, i));
                LCTEAddTreeEdge(e);
                break;
              }
            }
            if (!found) {
              edges.clear();
              LCTEUpdateInfo(u, Node(u));
            }
          }
        }
        if (found) break;
      }
    } else {
      LCTERemoveGraphEdge(edge);
      g.DeleteEdge(edge);
    }
  }

  unsigned Components() const { return ncomponents; }
};
}  // namespace connectivity
}  // namespace dynamic
}  // namespace graph
