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
  void LCTEAddEdge(TNode* from, TNode* to) {
    // std::cout << "\tLCTE_Add " << from->data << " " << to->data << std::endl;
    lcte.SetRoot(from);
    lcte.Link(from, to);
  }

  void LCTEAddEdge(unsigned from, unsigned to) {
    // std::cout << "\tLCTE Add " << from << " " << to << std::endl;
    LCTEAddEdge(lcte.Node(from), lcte.Node(to));
  }

  void LCTERemoveEdge(unsigned from, unsigned to) {
    // std::cout << "\tLCTE Rem " << from << " " << to << std::endl;
    lcte.SetRoot(lcte.Node(from));
    lcte.Cut(lcte.Node(to));
    assert(lcte.FindRoot(lcte.Node(to)) == lcte.Node(to));
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

  bool SameTree(TNode* node0, TNode* node1) {
    return lcte.FindRoot(node0) == lcte.FindRoot(node1);
  }

  TEdgeID InsertEdge(unsigned from, unsigned to) {
    // std::cout << "Insert Edge " << from << "\t" << to << std::endl;
    assert(from != to);
    auto e = g.AddEdge(from, to);
    auto n1 = Node(from), n2 = Node(to);
    if (!SameTree(n1, n2)) {
      --ncomponents;
      e->data.level = e->reversed_edge->data.level = 1;
      LCTEAddEdge(n1, n2);
    }
    return e;
  }

  void RemoveEdge(TEdgeID edge) {
    // std::cout << "Remove Edge " << edge->from << "\t" << edge->to <<
    // std::endl;
    if (edge->data.level & 1) {
      unsigned l = edge->data.level / 2;
      unsigned u1 = edge->from, u2 = edge->to;
      for (auto e = edge; e;) {
        LCTERemoveEdge(e->from, e->to);
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
            if ((e->data.level & 1) && !uset.HasKey(e->to)) {
              uset.Insert(e->to);
              s.push(e->to);
              if (e->data.level / 2 == l) {
                --j;
                e->reversed_edge->data.level = (e->data.level += 2);
                g.MoveEdge(e, e->from + size, e->to + size);
                LCTEAddEdge(e->from, e->to);
                auto enew = g.AddEdge(e->from - size, e->to - size,
                                      {2 * L + 3, e->data.prev_edge});
                e->reversed_edge->data.prev_edge = e->data.prev_edge =
                    static_cast<void*>(enew);
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
            auto v = e->to;
            auto rootv = lcte.FindRoot(lcte.Node(v));
            if (rootv == root1) {
              // Push level
              e->reversed_edge->data.level = (e->data.level += 2);
              g.MoveEdge(e, u + size, v + size);
              --j;
            } else {
              assert(rootv == lcte.FindRoot(node2));
              found = true;
              --ncomponents;
              e->reversed_edge->data.level = (e->data.level |= 1);
              LCTEAddEdge(e->from, e->to);
              for (auto eprev = e; eprev->from >= size;) {
                auto enext = g.AddEdge(eprev->from - size, eprev->to - size,
                                       {2 * L + 3, nullptr});
                eprev->reversed_edge->data.prev_edge = eprev->data.prev_edge =
                    enext;
                LCTEAddEdge(enext->from, enext->to);
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
