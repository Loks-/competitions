#pragma once

#include <queue>
#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Based on zero-degrees-only algorithm but without subtree disassenbly
// For directed graphs without negative cycle.
// Time: O(VE) worst case
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> ZeroDegreesOnlyBase(const TGraph& graph,
                                                  const TEdgeCostFunction& f,
                                                  unsigned source,
                                                  const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  std::vector<unsigned> inq(gsize, 0);
  for (q.push(source); !q.empty();) {
    unsigned u = q.front();
    q.pop();
    auto ucost = v[u];
    bool skip = false;
    for (const auto& e : graph.InvertedEdgesEI(u)) {
      if ((v[e.to] < max_cost) && (v[e.to] + f(e.info) < ucost)) {
        skip = true;
        break;
      }
    }
    if (skip) {
      inq[u] = 2;
      continue;
    }
    inq[u] = 0;
    for (const auto& e : graph.EdgesEI(u)) {
      if (ucost + f(e.info) < v[e.to]) {
        v[e.to] = ucost + f(e.info);
        if (inq[e.to] != 1) {
          inq[e.to] = 1;
          q.push(e.to);
        }
      }
    }
  }
  return v;
}
}  // namespace spfa
}  // namespace distance
}  // namespace graph
