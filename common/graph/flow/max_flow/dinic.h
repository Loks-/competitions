#pragma once

#include "common/graph/flow/graph.h"
#include <algorithm>
#include <functional>
#include <queue>
#include <vector>

namespace graph {
namespace flow {
namespace max_flow {
// Dinic's algorithm
// https://en.wikipedia.org/wiki/Dinic%27s_algorithm
// Time: O(V^2 E)
template <class TEdge>
inline typename TEdge::TFlow Dinic(Graph<TEdge>& g) {
  using TFlow = typename TEdge::TFlow;
  thread_local std::queue<unsigned> q;
  thread_local std::vector<unsigned> d;
  thread_local std::vector<unsigned> ei;
  unsigned source = g.Source(), sink = g.Sink();
  d.resize(g.Size());
  ei.resize(g.Size());
  TFlow source_max_flow = 0;
  for (auto& e : g.Edges(source))
    source_max_flow = std::max(source_max_flow, e.max_flow);

  std::function<TFlow(unsigned, TFlow)> AddBlockingFlow =
      [&](unsigned u, TFlow flow) -> TFlow {
    if (u == sink) return flow;
    unsigned du = d[u];
    auto& uedges = g.Edges(u);
    for (unsigned& i = ei[u]; i < uedges.size(); ++i) {
      auto& e = uedges[i];
      if ((d[e.to] == du + 1) && (e.flow < e.max_flow)) {
        auto flow_added =
            AddBlockingFlow(e.to, std::min(flow, e.max_flow - e.flow));
        if (flow_added) {
          e.flow += flow_added;
          g.ReversedEdge(e).flow -= flow_added;
          return flow_added;
        }
      }
    }
    return 0;
  };

  for (;;) {
    for (; !q.empty();) q.pop();
    std::fill(d.begin(), d.end(), -1u);
    d[source] = 0;
    for (q.push(source); (d[sink] == -1u) && !q.empty(); q.pop()) {
      unsigned u = q.front(), du = d[u];
      for (auto& e : g.Edges(u)) {
        if ((e.flow < e.max_flow) && (d[e.to] == -1u)) {
          d[e.to] = du + 1;
          if (e.to == sink) break;
          q.push(e.to);
        }
      }
    }
    if (d[sink] == -1u) break;
    std::fill(ei.begin(), ei.end(), 0);
    while (AddBlockingFlow(source, source_max_flow)) {
    }
  }
  return g.Flow();
}
}  // namespace max_flow
}  // namespace flow
}  // namespace graph
