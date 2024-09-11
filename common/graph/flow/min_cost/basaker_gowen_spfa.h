#pragma once

#include "common/base.h"
#include "common/graph/flow/distance/spfa.h"
#include "common/graph/flow/graph.h"

#include <algorithm>
#include <vector>

namespace graph {
namespace flow {
namespace min_cost {
// Basaker-Gowen algorithm with SPFA for path search.
// Time: O(FVE)
template <class TEdge, class TCostProxy>
inline typename TCostProxy::TCost BasakerGowenSPFA(
    Graph<TEdge>& g, const typename TEdge::TFlow& max_flow,
    const TCostProxy& f) {
  using TFlow = typename TEdge::TFlow;
  using TCost = typename TCostProxy::TCost;
  g.ResetFlow();
  const unsigned source = g.Source(), sink = g.Sink(), gsize = g.Size();
  std::vector<TEdge*> p(gsize);
  TFlow flow = TFlow(0);
  TCost cost = TCost(0);
  TEdge* pe = nullptr;
  for (; flow < max_flow;) {
    const auto d = distance::SPFA(g, f, source, sink, p);
    if (p[sink] == nullptr) break;
    auto new_flow = max_flow - flow;
    for (pe = p[sink]; pe; pe = p[pe->from])
      new_flow = std::min(new_flow, pe->max_flow - pe->flow);
    flow += new_flow;
    cost += d * new_flow;
    for (pe = p[sink]; pe; pe = p[pe->from]) {
      pe->flow += new_flow;
      g.ReversedEdge(*pe).flow -= new_flow;
    }
  }
  return cost;
}
}  // namespace min_cost
}  // namespace flow
}  // namespace graph
