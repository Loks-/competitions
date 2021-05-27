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
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  std::vector<unsigned> p(gsize, CNone);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::vector<unsigned> inq(gsize, 0);
  q.push(source);
  for (unsigned i = 0, ie = 0, r = 0; !q.empty(); ++i) {
    if (i == ie) {
      ie = i + q.size();
      ++r;
      if (r > gsize) break;
    }
    unsigned u = q.front();
    q.pop();
    inq[u] = 0;
    auto ucost = v[u];
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        p[e.to] = u;
        if (!inq[e.to]) {
          inq[e.to] = 1;
          q.push(e.to);
        }
      }
    }
  }
  if (q.empty()) return {};
  auto u0 = q.front();
  std::vector<unsigned> r(1, u0);
  for (auto u = p[u0]; u != u0; u = p[u]) r.push_back(u);
  std::reverse(r.begin(), r.end());
  return r;
}
}  // namespace ncycle
}  // namespace graph
