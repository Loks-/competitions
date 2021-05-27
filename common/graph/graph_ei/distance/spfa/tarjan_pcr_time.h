#pragma once

#include "common/graph/tree/pcr.h"

#include <queue>
#include <stack>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Modification of Tarjan negative-cycle detection algorithm.
// For graphs without negative cycle.
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> TarjanPCRTime(const TGraph& graph,
                                            const TEdgeCostFunction& f,
                                            unsigned source,
                                            const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::stack<unsigned> s;
  std::vector<unsigned> inq(gsize, 0);
  PCR pcr(gsize);
  q.push(source);
  for (unsigned t = 0, te = 0; !q.empty(); ++t) {
    unsigned u = q.front();
    q.pop();
    if (inq[u] != t) continue;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        unsigned u2 = e.to;
        if (inq[u2] > t) {
          // Remove subtree from queue
          for (auto c : pcr(u2).childs)
            if (inq[c] > t) s.push(c);
          for (; !s.empty();) {
            unsigned x = s.top();
            s.pop();
            inq[x] = 0;
            for (auto c : pcr(x).childs)
              if (inq[c] > t) s.push(c);
          }
        }
        v[u2] = ucost + f(e.info);
        pcr.SetP(u2, u);
        inq[u2] = ++te;
        q.push(u2);
      }
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
