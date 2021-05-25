#pragma once

#include "common/graph/flow/graph.h"

#include <algorithm>
#include <queue>
#include <vector>

namespace graph {
namespace flow {
namespace max_flow {
// Edmonds-Karp algorithm
// https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm
// Time: O(VE^2)
template <class TEdge>
inline typename TEdge::TFlow EdmondsKarp(Graph<TEdge>& g) {
  std::queue<unsigned> q;
  std::vector<TEdge*> p;
  unsigned source = g.Source(), sink = g.Sink();
  p.resize(g.Size());
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
  }
  return g.Flow();
}
}  // namespace max_flow
}  // namespace flow
}  // namespace graph
