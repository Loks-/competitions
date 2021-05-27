#pragma once

#include "common/graph/cnone.h"

#include <algorithm>
#include <queue>
#include <vector>

namespace graph {
namespace ncycle {
// Modification for SPFA to catch negative cycle.
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<unsigned> SPFA(const TGraph& graph,
                                  const TEdgeCostFunction& f, unsigned source,
                                  const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size(), u0 = CNone;
  std::vector<TEdgeCost> v(gsize, max_cost);
  std::vector<unsigned> p(gsize, CNone), l(gsize, 0);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::vector<unsigned> inq(gsize, 0);
  q.push(source);
  for (; !q.empty();) {
    unsigned u = q.front();
    q.pop();
    inq[u] = 0;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        p[e.to] = u;
        l[e.to] = l[u] + 1;
        if (l[e.to] >= gsize) {
          u0 = e.to;
          break;
        }
        if (!inq[e.to]) {
          inq[e.to] = 1;
          q.push(e.to);
        }
      }
    }
  }
  if (u0 == CNone) return {};
  std::vector<unsigned> r(1, u0);
  for (auto u = p[u0]; u != u0; u = p[u]) r.push_back(u);
  std::reverse(r.begin(), r.end());
  return r;
}
}  // namespace ncycle
}  // namespace graph
