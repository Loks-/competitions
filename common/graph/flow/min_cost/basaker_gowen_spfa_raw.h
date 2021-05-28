#pragma once

#include "common/base.h"
#include "common/graph/flow/graph.h"

#include <algorithm>
#include <queue>
#include <vector>

namespace graph {
namespace flow {
namespace min_cost {
// Basaker-Gowen algorithm with SPFA for path search.
// Time: O(FVE)
template <class TEdge, class TCostProxy>
inline typename TCostProxy::TCost BasakerGowenSPFARaw(
    Graph<TEdge>& g, const typename TEdge::TFlow& max_flow,
    const TCostProxy& f) {
  using TFlow = typename TEdge::TFlow;
  using TCost = typename TCostProxy::TCost;
  g.ResetFlow();
  unsigned source = g.Source(), sink = g.Sink(), gsize = g.Size();
  std::vector<TCost> v(gsize);
  std::vector<TEdge*> p(gsize);
  std::queue<unsigned> q;
  std::vector<unsigned> inq(gsize);
  TFlow flow = TFlow(0);
  TCost cost = TCost(0);
  TEdge* pe = nullptr;
  for (; flow < max_flow;) {
    std::fill(p.begin(), p.end(), nullptr);
    std::fill(inq.begin(), inq.end(), 0);
    for (q.push(source); !q.empty();) {
      unsigned u = q.front();
      q.pop();
      inq[u] = 2;
      auto ucost = v[u];
      for (auto& e : g.Edges(u)) {
        if ((e.to != source) && (e.flow < e.max_flow)) {
          if ((inq[e.to] == 0) || (ucost + f(e) < v[e.to])) {
            v[e.to] = ucost + f(e);
            p[e.to] = &e;
            if (inq[e.to] != 1) {
              inq[e.to] = 1;
              q.push(e.to);
            }
          }
        }
      }
    }
    if (inq[sink] == 0) break;
    auto new_flow = max_flow - flow;
    for (pe = p[sink]; pe; pe = p[pe->from])
      new_flow = std::min(new_flow, pe->max_flow - pe->flow);
    flow += new_flow;
    cost += v[sink] * new_flow;
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
