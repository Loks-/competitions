#pragma once

#include "common/graph/graph_ei.h"

#include <queue>
#include <vector>

namespace graph {
namespace distance {
// Shortest Path Faster Algorithm + Large Label Last.
// https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm
// Assume that no negative cycles exists
// Time: O(VE) worst case, O(E) on random graph
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> SPFALLL(const TGraph& graph,
                                          const TEdgeCostFunction& f,
                                          unsigned source,
                                          const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::queue<unsigned> q;
  TEdgeCost qs = v[source];
  std::vector<unsigned> inq(gsize, 0);
  for(q.push(source); !q.empty();) {
      unsigned u = q.front();
      q.pop();
      auto ucost = v[u];
      if (ucost * TEdgeCost(q.size() + 1) > qs) {
        q.push(u);
        continue;
      }
      inq[u] = 0;
      qs -= ucost;
      for (const auto& e : graph.EdgesEI(u)) {
        if (ucost + f(e.info) < v[e.to]) {
          auto vold = v[e.to];          
          v[e.to] = ucost + f(e.info);
          if (!inq[e.to]) {
            inq[e.to] = 1;
            q.push(e.to);
            qs += v[e.to];
          } else {
            qs -= (vold - v[e.to]);
          }
        }
      }
  }
  return v;
}
}  // namespace distance
}  // namespace graph
