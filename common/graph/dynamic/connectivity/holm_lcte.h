#pragma once

#include "common/base.h"
#include "common/data_structures/unsigned_set.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/tree/lcte/action/none.h"
#include "common/graph/tree/lcte/info/size.h"
#include "common/graph/tree/lcte/lcte.h"
#include "common/numeric/bits/ulog2.h"
#include "common/vector/enumerate.h"

#include <algorithm>
#include <iostream>
#include <stack>

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
    unsigned level = 0;
    void* prev_edge = nullptr;

    EdgeData() {}
    EdgeData(unsigned _level, void* _prev_edge)
        : level(_level), prev_edge(_prev_edge) {}
  };

  using TGraph = graph::dynamic::Graph<EdgeData>;
  using TEdge = typename TGraph::TEdge;
  using TEdgeID = TEdge*;
  using TLCTE =
      graph::lcte::LCTE<unsigned, graph::lcte::info::PSize,
                        graph::lcte::info::VSize, graph::lcte::action::None>;
  using TNode = typename TLCTE::TPNode;

 protected:
  unsigned size;
  unsigned L;
  TGraph g;
  TLCTE lcte;
  unsigned ncomponents;
  ds::UnsignedSet uset;

 protected:
  void LCTEAddEdge(TNode* node1, TNode* node2) {
    lcte.SetRoot(node1);
    lcte.Link(node1, node2);
  }

  void LCTEAddEdge(unsigned u1, unsigned u2) {
    LCTEAddEdge(lcte.Node(u1), lcte.Node(u2));
  }

  void LCTERemoveEdge(unsigned u1, unsigned u2) {
    lcte.SetRoot(lcte.Node(u1));
    lcte.Cut(lcte.Node(u2));
    assert(lcte.FindRoot(lcte.Node(u2)) == lcte.Node(u2));
  }

 public:
  HolmLCTE(unsigned _size)
      : size(_size),
        L(numeric::ULog2(size) + 2),
        g(L * size),
        lcte(nvector::Enumerate<unsigned>(0, L * size)),
        ncomponents(size),
        uset(L * size) {}

  TNode* Node(unsigned index) { return lcte.Node(index); }

  bool SameTree(TNode* node1, TNode* node2) {
    return lcte.FindRoot(node1) == lcte.FindRoot(node2);
  }

  TEdgeID InsertEdge(unsigned u1, unsigned u2) {
    // std::cout << "Insert Edge " << from << "\t" << to << std::endl;
    assert(u1 != u2);
    auto e = g.AddEdge(u1, u2);
    auto n1 = Node(u1), n2 = Node(u2);
    if (!SameTree(n1, n2)) {
      --ncomponents;
      e->data.level = 1;
      LCTEAddEdge(n1, n2);
    }
    return e;
  }

  void RemoveEdge(TEdgeID edge) {
    // std::cout << "Remove Edge " << edge->from << "\t" << edge->to <<
    // std::endl;
    if (edge->data.level & 1) {
      unsigned l = edge->data.level / 2;
      unsigned u1 = edge->u1, u2 = edge->u2;
      for (auto e = edge; e;) {
        LCTERemoveEdge(e->u1, e->u2);
        auto enext = static_cast<TEdgeID>(e->data.prev_edge);
        g.DeleteEdge(e);
        e = enext;
      }
      ++ncomponents;
      for (++l; l-- > 0;) {
        bool found = false;
        auto node1 = lcte.Node(u1), node2 = lcte.Node(u2);
        lcte.Access(node1);
        lcte.Access(node2);
        if (node1->info.tsize > node2->info.tsize) {
          std::swap(u1, u2);
          std::swap(node1, node2);
        }
        uset.Clear();
        uset.Insert(u1);
        // Push ST edges up
        thread_local std::stack<unsigned> s;
        for (s.push(u1); !s.empty();) {
          auto u = s.top();
          s.pop();
          for (unsigned j = 0; j < g.Edges(u).size(); ++j) {
            auto e = g.Edges(u)[j];
            auto u2 = e->Other(u);
            if ((e->data.level & 1) && !uset.HasKey(u2)) {
              uset.Insert(u2);
              s.push(u2);
              if (e->data.level / 2 == l) {
                --j;
                e->data.level += 2;
                g.MoveEdge(e, e->u1 + size, e->u2 + size);
                LCTEAddEdge(e->u1, e->u2);
                auto enew = g.AddEdge(e->u1 - size, e->u2 - size,
                                      {2 * L + 3, e->data.prev_edge});
                e->data.prev_edge = static_cast<void*>(enew);
              }
            }
          }
        }
        // Check other edges
        auto root1 = lcte.FindRoot(node1);
        for (auto u : uset.List()) {
          for (unsigned j = 0; j < g.Edges(u).size(); ++j) {
            auto e = g.Edges(u)[j];
            if (e->data.level & 1) continue;
            auto v = e->Other(u);
            auto rootv = lcte.FindRoot(lcte.Node(v));
            if (rootv == root1) {
              // Push level
              e->data.level += 2;
              g.MoveEdge(e, u + size, v + size);
              --j;
            } else {
              assert(rootv == lcte.FindRoot(node2));
              found = true;
              --ncomponents;
              e->data.level |= 1;
              LCTEAddEdge(e->u1, e->u2);
              for (auto eprev = e; eprev->u1 >= size;) {
                auto enext = g.AddEdge(eprev->u1 - size, eprev->u2 - size,
                                       {2 * L + 3, nullptr});
                eprev->data.prev_edge = enext;
                LCTEAddEdge(enext->u1, enext->u2);
                eprev = enext;
              }
              break;
            }
          }
          if (found) break;
        }
        if (found) break;
        u1 -= size;
        u2 -= size;
      }
    } else {
      g.DeleteEdge(edge);
    }
  }

  unsigned Components() const { return ncomponents; }
};
}  // namespace connectivity
}  // namespace dynamic
}  // namespace graph
