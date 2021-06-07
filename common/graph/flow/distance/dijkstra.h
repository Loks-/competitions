#pragma once

#include "common/graph/flow/graph.h"

#include <algorithm>
#include <vector>

namespace graph {
namespace flow {
namespace distance {
// Dijkstra's algorithm with potentials (u)
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// Time: O((V + E) log V)
template <class TEdge, class TCostProxy, class TCost, class TKVMHeap>
void Dijkstra(Graph<TEdge>& g, const TCostProxy& f,
              const std::vector<TCost>& vu, TKVMHeap& q, unsigned source,
              std::vector<TEdge*>& vp) {
  thread_local std::vector<unsigned> vf;
  unsigned gsize = g.Size();
  if (vp.size() != gsize) vp.resize(gsize);
  std::fill(vp.begin(), vp.end(), nullptr);
  for (q.AddNewKey(source, TCost()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    TCost ucost = q.Get(u) + vu[u];
    for (auto e : g.Edges(u)) {
      if ((e.to != source) && (e.flow < e.max_flow)) {
        auto ecost = ucost + f(e) - vu[e.to];
        if ((vp[e.to] == nullptr) || (ecost < q.Get(e.to))) {
          vp[e.to] = &e;
          q.Set(e.to, ecost);
        }
      }
    }
  }
}
}  // namespace distance
}  // namespace flow
}  // namespace graph
