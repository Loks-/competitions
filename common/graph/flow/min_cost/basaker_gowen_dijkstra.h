#pragma once

#include "common/base.h"
#include "common/graph/flow/distance/spfa.h"
#include "common/graph/flow/distance/dijkstra.h"
#include "common/graph/flow/graph.h"

#include <algorithm>
#include <vector>

namespace graph {
namespace flow {
namespace min_cost {
// Basaker-Gowen algorithm with Dijkstra for path search.
// Time: O(F (V+E) log V + VE)
template <class TEdge, class TCostProxy, class TKVMHeap>
inline typename TCostProxy::TCost BasakerGowenDijkstra(
    Graph<TEdge>& g, const typename TEdge::TFlow& max_flow,
    const TCostProxy& f, TKVMHeap& q) {
  using TFlow = typename TEdge::TFlow;
  using TCost = typename TCostProxy::TCost;
  g.ResetFlow();
  unsigned source = g.Source(), sink = g.Sink(), gsize = g.Size();
  std::vector<TEdge*> vp(gsize);
  std::vector<TCost> vu(gsize, TCost());
  bool vu_init_req = false;
  for (unsigned u = 0; (u < gsize) && !vu_init_req; ++u) {
    for (auto e : g.Edges(u)) {
      if ((e.flow < e.max_flow) && (f(e) < TCost())) {
        vu_init_req = true;
        break;
      }
    }
  }
  if (vu_init_req) {
    // ...
  }

  TFlow flow = TFlow(0);
  TCost cost = TCost(0);
  TEdge* pe = nullptr;
  for (; flow < max_flow;) {
    distance::Dijkstra(g, f, vu, q, source, vp);
    if (vp[sink] == nullptr) break;
    auto d = q.Get(sink) + vu[sink];
    for (unsigned u = 0; u < gsize; ++u) {
      if (vp[u])
        vu[u] = q.Get(u) + vu[u];
    }
    auto new_flow = max_flow - flow;
    for (pe = vp[sink]; pe; pe = vp[pe->from])
      new_flow = std::min(new_flow, pe->max_flow - pe->flow);
    flow += new_flow;
    cost += d * new_flow;
    for (pe = vp[sink]; pe; pe = vp[pe->from]) {
      pe->flow += new_flow;
      g.ReversedEdge(*pe).flow -= new_flow;
    }
  }
  return cost;
}
}  // namespace min_cost
}  // namespace flow
}  // namespace graph
