#pragma once

#include "common/graph/graph_ei/distance/spfa/parent_tree.h"

#include <queue>
#include <stack>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Zero-degrees-only algorithm.
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> ZeroDegreesOnly(const TGraph& graph,
                                              const TEdgeCostFunction& f,
                                              unsigned source,
                                              const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::stack<unsigned> s;
  std::vector<unsigned> inq(gsize, 0);
  ParentTree pt(gsize);
  pt.SetRoot(source);
  inq[source] = 1;
  for (q.push(source); !q.empty();) {
    unsigned u = q.front();
    q.pop();
    if (inq[u] != 1) continue;
    auto ucost = v[u];
    bool skip = false;
    for (const auto& e : graph.InvertedEdgesEI(u)) {
      if ((v[e.to] < max_cost) && (v[e.to] + f(e.info) < ucost)) {
        skip = true;
        break;
      }
    }
    inq[u] = 0;
    if (skip) continue;
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        unsigned u2 = e.to, p2 = pt.vp[u2];
        if (p2 != CNone) {
          // Disassemble subtree
          pt.vc[p2] -= 1;
          auto ul = pt.vl[u2], u3 = u2;
          for (unsigned k = 1; k > 0; --k) {
            k += pt.vc[u3];
            pt.Reset(u3);
            if (u3 == u) return {};  // Negative cycle
            inq[u3] = 0;
            u3 = pt.vr[u3];
          }
          pt.vr[ul] = u3;
          pt.vl[u3] = ul;
        }
        v[u2] = ucost + f(e.info);
        pt.SetParent(u2, u);
        if (inq[u2] != 1) {
          inq[u2] = 1;
          q.push(u2);
        }
      }
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
