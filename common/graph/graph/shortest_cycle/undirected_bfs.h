#pragma once

#include "common/base.h"
#include "common/data_structures/unsigned_set.h"
#include "common/graph/dynamic/convert.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/graph.h"
#include "common/heap/ukvm/dheap.h"
#include "common/template.h"

#include <algorithm>
#include <queue>
#include <vector>

namespace graph {
namespace scycle {
// Find shortest cycle length by running BFS from each node.
// Time: O(V (V + E))
namespace hidden {
inline void BFSI(const graph::dynamic::UndirectedGraph<TEmpty>& g,
                 unsigned source, unsigned& best_cost, std::queue<unsigned>& q,
                 std::vector<unsigned>& vd, ds::UnsignedSet& us,
                 std::vector<unsigned>& vf) {
  const unsigned none = -1;
  // Run BFS
  us.Insert(source);
  vf[source] = none;
  vd[source] = 0;
  for (q.push(source); !q.empty(); q.pop()) {
    unsigned u = q.front();
    auto ucost = vd[u];
    if (2 * ucost + 1 >= best_cost) {
      for (; !q.empty();) q.pop();
      break;
    }
    for (auto e : g.Edges(u)) {
      auto v = e->Other(u);
      if (v == vf[u]) continue;
      if (vd[v] != none) {
        best_cost = std::min(best_cost, vd[v] + ucost + 1u);
      } else {
        us.Insert(v);
        vf[v] = u;
        vd[v] = ucost + 1;
        q.push(v);
      }
    }
  }
  // Restore internal data
  for (auto u : us.List()) vd[u] = none;
  us.Clear();
}
}  // namespace hidden

inline unsigned BFS(const UndirectedGraph& g) {
  unsigned best_cost = unsigned(-1);
  for (unsigned u = 0; u < g.Size(); ++u) {
    for (auto v : g.Edges(u)) {
      if (v == u) return 1;
    }
  }
  std::queue<unsigned> q;
  ds::UnsignedSet us(g.Size());
  std::vector<unsigned> vd(g.Size(), best_cost), vf(g.Size());
  graph::dynamic::UndirectedGraph<TEmpty> dg;
  graph::dynamic::Convert(g, dg, {});
  std::vector<unsigned> vpc(g.Size());
  for (unsigned i = 0; i < vpc.size(); ++i) vpc[i] = dg.Edges(i).size();
  heap::ukvm::DHeap<4u, unsigned, std::greater<unsigned>> qe(vpc, false);
  for (; !qe.Empty();) {
    auto u = qe.ExtractKey();
    if (qe.Get(u) == 0) break;
    hidden::BFSI(dg, u, best_cost, q, vd, us, vf);
    auto edges = dg.Edges(u);
    std::reverse(edges.begin(), edges.end());
    for (auto& e : edges) {
      auto v = e->Other(u);
      dg.DeleteEdge(e);
      qe.Set(v, dg.Edges(v).size());
    }
  }
  return best_cost;
}
}  // namespace scycle
}  // namespace graph
