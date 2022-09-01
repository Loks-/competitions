#pragma once

#include "common/base.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/tree/lcte/action/none.h"
#include "common/graph/tree/lcte/info/size.h"
#include "common/graph/tree/lcte/lcte.h"
#include "common/numeric/bits/ulog2.h"
#include "common/template.h"

#include <algorithm>
#include <iostream>
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

    EdgeData() : level(0) {}
  };

  using TGraph = graph::dynamic::Graph<EdgeData>;
  using TEdge = typename TGraph::TEdge;
  using TEdgeID = TEdge*;

  class LCTENodeData {
   public:
    std::vector<TEdgeID> tree_edges;
    std::vector<TEdgeID> graph_edges;
  };

  class LCTEPInfo : public graph::lcte::info::PSize {
   public:
    using TBase = graph::lcte::info::PSize;

    TEdgeID dedge;
    TEdgeID tedge;

    template <class TNode>
    void DUpdate(TNode* node) {
      if (!node->data.tree_edges.empty()) {
        dedge = node->data.tree_edges.back();
      } else if (!node->data.graph_edges.empty()) {
        dedge = node->data.graph_edges.back();
      } else {
        dedge = nullptr;
      }
    }

    template <class TNode>
    void PUpdate(TNode* node) {
      TBase::PUpdate(node);
      tedge = dedge;
      if (node->l && node->l->info.tedge) {
        if (!tedge || (tedge->data.level < node->l->info.tedge->data.level))
          tedge = node->l->info.tedge;
      }
      if (node->r && node->r->info.tedge) {
        if (!tedge || (tedge->data.level < node->r->info.tedge->data.level))
          tedge = node->r->info.tedge;
      }
      if (node->vc && node->vc->info.tedge) {
        if (!tedge || (tedge->data.level < node->vc->info.tedge->data.level))
          tedge = node->vc->info.tedge;
      }
    }
  };

  class LCTEVInfo : public graph::lcte::info::VSize {
   public:
    using TBase = graph::lcte::info::VSize;

    TEdgeID tedge;

    template <class TNode>
    void VUpdate(TNode* node) {
      TBase::VUpdate(node);
      assert(node->vc);
      tedge = node->vc->info.tedge;
      if (node->l && node->l->info.tedge) {
        if (!tedge || (tedge->data.level < node->l->info.tedge->data.level))
          tedge = node->l->info.tedge;
      }
      if (node->r && node->r->info.tedge) {
        if (!tedge || (tedge->data.level < node->r->info.tedge->data.level))
          tedge = node->r->info.tedge;
      }
    }
  };

  using TLCTE = graph::lcte::LCTE<LCTENodeData, LCTEPInfo, LCTEVInfo,
                                  graph::lcte::action::None>;
  using TNode = typename TLCTE::TPNode;

 protected:
  unsigned size;
  unsigned L;
  TGraph g;
  TLCTE lcte;
  unsigned ncomponents;

 protected:
  void LCTEUpdateInfo(TNode* node) {
    // lcte.UpdateNodeToRoot(node);
    node->info.DUpdate(node);
    lcte.Access(node);
    // node->UpdateInfo();
  }

  void LCTEAddTreeEdge(TEdgeID edge, TNode* node1) {
    // std::cout << "\tATE:\t" << edge->u1 << "\t" << edge->u2 << "\t"
    //           << edge->data.level << std::endl;
    node1->data.tree_edges.push_back(edge);
    if (node1->data.tree_edges.size() == 1)
      LCTEUpdateInfo(node1);
  }

  void LCTEAddGraphEdge(TEdgeID edge, TNode* node1, TNode* node2) {
    // std::cout << "\tAGE:\t" << edge->u1 << "\t" << edge->u2 << "\t"
    //           << edge->data.level << std::endl;
    node1->data.graph_edges.push_back(edge);
    node2->data.graph_edges.push_back(edge);
    if (node1->data.tree_edges.empty() && (node1->data.graph_edges.size() == 1))
      LCTEUpdateInfo(node1);
    if (node2->data.tree_edges.empty() && (node2->data.graph_edges.size() == 1))
      LCTEUpdateInfo(node2);
  }

  void LCTEAddTreeEdge(TEdgeID edge) {
    unsigned l = edge->data.level / 2;
    LCTEAddTreeEdge(edge, Node(edge->u1, l));
  }

  void LCTEAddGraphEdge(TEdgeID edge) {
    unsigned l = edge->data.level / 2;
    LCTEAddGraphEdge(edge, Node(edge->u1, l), Node(edge->u2, l));
  }

  void LCTERemoveTreeEdge(TEdgeID edge, TNode* node1) {
    // std::cout << "\tRTE:\t" << edge->u1 << "\t" << edge->u2 << "\t"
    //           << edge->data.level << std::endl;
    std::vector<TEdgeID>& edges = node1->data.tree_edges;
    bool found = false;
    for (unsigned i = 0; i < edges.size(); ++i) {
      if (edges[i] == edge) {
        found = true;
        if (i + 1 < edges.size()) {
          edges[i] = edges.back();
          // ...
        }
        edges.pop_back();
        if (node1->info.dedge == edge)
          LCTEUpdateInfo(node1);
        break;
      }
    }
    FakeUse(found);
    assert(found);
  }

  void LCTERemoveGraphEdge(TEdgeID edge, TNode* node1, TNode* node2) {
    // std::cout << "\tRGE:\t" << edge->u1 << "\t" << edge->u2 << "\t"
    //           << edge->data.level << std::endl;
    for (auto node : {node1, node2}) {
      std::vector<TEdgeID>& edges = node->data.graph_edges;
      bool found = false;
      for (unsigned i = 0; i < edges.size(); ++i) {
        if (edges[i] == edge) {
          found = true;
          if (i + 1 < edges.size()) {
            edges[i] = edges.back();
            // ...
          }
          edges.pop_back();
          if (node->info.dedge == edge)
            LCTEUpdateInfo(node);
          break;
        }
      }
      FakeUse(found);
      assert(found);
    }
  }

  void LCTERemoveTreeEdge(TEdgeID edge) {
    unsigned l = edge->data.level / 2;
    LCTERemoveTreeEdge(edge, Node(edge->u1, l));
  }

  void LCTERemoveGraphEdge(TEdgeID edge) {
    unsigned l = edge->data.level / 2;
    LCTERemoveGraphEdge(edge, Node(edge->u1, l), Node(edge->u2, l));
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
        g(size),
        lcte(L * size),
        ncomponents(size) {}

  TNode* Node(unsigned index) { return lcte.Node(index); }

  TNode* Node(unsigned index, unsigned level) {
    return Node(index + size * level);
  }

  bool SameTree(TNode* node1, TNode* node2) {
    return lcte.FindRoot(node1) == lcte.FindRoot(node2);
  }

  TEdgeID InsertEdge(unsigned u1, unsigned u2) {
    // std::cout << "Insert Edge " << u1 << "\t" << u2 << std::endl;
    assert(u1 != u2);
    auto e = g.AddEdge(u1, u2);
    auto n1 = Node(u1), n2 = Node(u2);
    if (SameTree(n1, n2)) {
      LCTEAddGraphEdge(e, n1, n2);
    } else {
      --ncomponents;
      e->data.level = 1;
      LCTELinkEdge(n1, n2);
      LCTEAddTreeEdge(e, n1);
    }
    return e;
  }

  void RemoveEdge(TEdgeID edge) {
    // std::cout << "Remove Edge " << edge->u1 << "\t" << edge->u2 << std::endl;
    if (edge->data.level & 1) {
      unsigned l = edge->data.level / 2;
      unsigned u1 = edge->u1, u2 = edge->u2;
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
        // std::cout << "\t\tRE:\t" << u1 << "\t" << u2 << "\t" << l << "\t"
        //           << node1->info.tsize << "\t" << node2->info.tsize
        //           << std::endl;
        if (node1->info.tsize > node2->info.tsize) {
          std::swap(node1, node2);
        }
        lcte.SetRoot(node1);
        auto root2 = lcte.FindRoot(node2);
        FakeUse(root2);
        assert(root2 != node1);
        for (; node1->info.tedge; lcte.Access(node1)) {
          auto e = node1->info.tedge;
          // if (e->data.level / 2 != l) {
          //   std::cout << "WRONG.\tL = " << l << "\tEL = " << e->data.level
          //             << "\tS = " << node1->info.tsize
          //             << "\tI = " << node1 - Node(0) << std::endl;
          //   // node1->UpdateInfo();
          //   lcte.UpdateTreeInfo(node1);
          //   auto e2 = node1->info.tedge;
          //   if (e2) std::cout << "\tE2L = " << e2->data.level << std::endl;
          // }
          if (e->data.level & 1) {
            // Push tree edge up
            LCTERemoveTreeEdge(e);
            e->data.level += 2;
            auto n1 = Node(e->u1, l + 1), n2 = Node(e->u2, l + 1);
            LCTELinkEdge(n1, n2);
            LCTEAddTreeEdge(e, n1);
          } else {
            auto n1 = Node(e->u1, l), n2 = Node(e->u2, l);
            auto r1 = lcte.FindRoot(n1), r2 = lcte.FindRoot(n2);
            if (r1 == r2) {
              assert(r1 == node1);
              LCTERemoveGraphEdge(e, n1, n2);
              e->data.level += 2;
              LCTEAddGraphEdge(e);
            } else {
              assert(r1 - node1 + r2 == root2);
              found = true;
              --ncomponents;
              LCTERemoveGraphEdge(e, n1, n2);
              e->data.level += 1;
              for (unsigned i = 0; i <= l; ++i)
                LCTELinkEdge(Node(e->u1, i), Node(e->u2, i));
              LCTEAddTreeEdge(e);
              break;
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
