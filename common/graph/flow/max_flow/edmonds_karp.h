#pragma once

#include "common/graph/flow/graph.h"
#include <algorithm>
#include <queue>
#include <vector>

namespace graph {
namespace flow {
template <class TEdge>
inline typename TEdge::TFlow MaxFlowEdmondsKarp(Graph<TEdge>& g) {
  thread_local std::queue<unsigned> q;
  thread_local std::vector<TEdge*> p;
  unsigned source = g.Source(), sink = g.Sink();
  p.resize(g.Size());
  auto f = g.Flow();
  for (;;) {
    for (; !q.empty();) q.pop();
    std::fill(p.begin(), p.end(), nullptr);
    for (q.push(source); !p[sink] && !q.empty(); q.pop()) {
      unsigned u = q.front();
      for (auto& e : g.Edges(u)) {
        if ((e.to != source) && (e.flow < e.max_flow) && (p[e.to] == nullptr)) {
          p[e.to] = &e;
          if (e.to == sink) break;
          q.push(e.to);
        }
      }
    }
    if (p[sink] == nullptr) break;
    TEdge* pe = p[sink];
    auto new_flow = pe->max_flow - pe->flow;
    for (pe = p[pe->from]; pe; pe = p[pe->from])
      new_flow = std::min(new_flow, pe->max_flow - pe->flow);
    for (pe = p[sink]; pe; pe = p[pe->from]) {
      pe->flow += new_flow;
      g.ReversedEdge(*pe).flow -= new_flow;
    }
    f += new_flow;
  }
  return f;
}
}  // namespace flow
}  // namespace graph
