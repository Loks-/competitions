#pragma once

#include "common/graph/flow/graph.h"

#include <algorithm>
#include <queue>
#include <vector>

namespace graph {
namespace flow {
namespace distance {
// Time O(VE) worst case.
template <class TEdge, class TCostProxy>
inline typename TCostProxy::TCost SPFA(Graph<TEdge>& g, const TCostProxy& f,
                                       unsigned source, unsigned target,
                                       std::vector<TEdge*>& vp) {
  using TCost = typename TCostProxy::TCost;
  thread_local std::vector<TCost> vd;
  thread_local std::vector<unsigned> inq;
  thread_local std::queue<unsigned> q;
  const unsigned gsize = g.Size();
  if (vp.size() != gsize) vp.resize(gsize);
  if (vd.size() != gsize) vd.resize(gsize);
  if (inq.size() != gsize) inq.resize(gsize);
  vd[source] = TCost();
  std::fill(vp.begin(), vp.end(), nullptr);
  std::fill(inq.begin(), inq.end(), 0);
  for (q.push(source); !q.empty();) {
    const unsigned u = q.front();
    q.pop();
    inq[u] = 2;
    const auto ucost = vd[u];
    for (auto& e : g.Edges(u)) {
      if ((e.to != source) && (e.flow < e.max_flow)) {
        if ((inq[e.to] == 0) || (ucost + f(e) < vd[e.to])) {
          vd[e.to] = ucost + f(e);
          vp[e.to] = &e;
          if (inq[e.to] != 1) {
            inq[e.to] = 1;
            q.push(e.to);
          }
        }
      }
    }
  }
  return inq[target] ? vd[target] : TCost();
}
}  // namespace distance
}  // namespace flow
}  // namespace graph
