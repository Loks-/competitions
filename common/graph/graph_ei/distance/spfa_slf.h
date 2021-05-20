#pragma once

#include "common/graph/graph_ei.h"

#include <deque>
#include <vector>

namespace graph {
namespace distance {
// Shortest Path Faster Algorithm + SLF.
// https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm
// Assume that no negative cycles exists
// Time: O(VE) worst case, O(E) on random graph
template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
inline std::vector<TEdgeCost> SPFASLF(const TGraph& graph,
                                          const TEdgeCostFunction& f,
                                          unsigned source,
                                          const TEdgeCost& max_cost) {
  unsigned gsize = graph.Size();
  std::vector<TEdgeCost> v(gsize, max_cost);
  v[source] = TEdgeCost();
  std::deque<unsigned> q;
  std::vector<unsigned> inq(gsize, 0);
  for(q.push_back(source); !q.empty();) {
      unsigned u = q.front();
      q.pop_front();
      inq[u] = 0;
      auto ucost = v[u];
      for (const auto& e : graph.EdgesEI(u)) {
        if (ucost + f(e.info) < v[e.to]) {
          v[e.to] = ucost + f(e.info);
          if (!inq[e.to]) {
            inq[e.to] = 1;
            if (q.empty() || (v[q.front()] > v[e.to]))
              q.push_front(e.to);
            else
              q.push_back(e.to);
          }
        }
      }
  }
  return v;
}
}  // namespace distance
}  // namespace graph
