#pragma once

#include "common/graph/tree/pcr.h"

#include <queue>
#include <stack>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Tarjan negative-cycle detection algorithm.
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> TarjanPCR(const TGraph& graph,
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
  inq[source] = 1;
  for (q.push(source); !q.empty();) {
    unsigned u = q.front();
    q.pop();
    if (inq[u] != 1) continue;
    inq[u] = 2;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        unsigned t = e.to;
        if (inq[t]) {
          // Remove subtree from queue
          for (auto c : pcr(t).childs)
            if (inq[c]) s.push(c);
          for (; !s.empty();) {
            unsigned x = s.top();
            s.pop();
            inq[x] = 0;
            if (x == u) return {};  // Negative cycle
            for (auto c : pcr(x).childs)
              if (inq[c]) s.push(c);
          }
        }
        v[t] = ucost + f(e.info);
        pcr.SetP(t, u);
        if (inq[t] != 1) {
          inq[t] = 1;
          q.push(t);
        }
      }
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
