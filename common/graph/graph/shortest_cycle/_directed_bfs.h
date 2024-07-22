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

// Code in this file was not tested.

namespace graph {
namespace scycle {
// Find shortest cycle length by running BFS from each node.
// Time: O(V (V + E))
namespace hidden {
inline void BFSI(const graph::dynamic::DirectedGraph<TEmpty>& g,
                 unsigned source, unsigned& best_cost, std::queue<unsigned>& q,
                 std::vector<unsigned>& vd, ds::UnsignedSet& us) {
  // Run Dijkstra
  const unsigned none = -1;
  // Run BFS
  us.Insert(source);
  vd[source] = 0;
  for (q.push(source); !q.empty(); q.pop()) {
    unsigned u = q.front();
    auto ucost = vd[u];
    if (ucost + 1 >= best_cost) {
      for (; !q.empty();) q.pop();
      break;
    }
    for (auto e : g.Edges(u)) {
      auto v = e->u2;
      if (v == source) {
        best_cost = ucost + 1;
        break;
      } else if (vd[v] == none) {
        us.Insert(v);
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

inline unsigned BFS(const DirectedGraph& g) {
  unsigned best_cost = unsigned(-1);
  std::queue<unsigned> q;
  std::vector<unsigned> vd(g.Size(), best_cost);
  ds::UnsignedSet us(g.Size());
  graph::dynamic::DirectedGraph<TEmpty> dg;
  graph::dynamic::Convert(g, dg, {});
  std::vector<unsigned> vpc(g.Size());
  for (unsigned i = 0; i < vpc.size(); ++i)
    vpc[i] = dg.Edges(i).size() * dg.InvertedEdges(i).size();
  heap::ukvm::DHeap<4u, unsigned, std::greater<unsigned>> qe(vpc, false);
  for (; !qe.Empty();) {
    auto u = qe.ExtractKey();
    if (qe.Get(u) == 0) break;
    hidden::BFSI(dg, u, best_cost, q, vd, us);
    for (auto edges : {dg.Edges(u), dg.InvertedEdges(u)}) {
      std::reverse(edges.begin(), edges.end());
      for (auto& e : edges) {
        auto v = e->Other(u);
        dg.DeleteEdge(e);
        us.Insert(v);
      }
    }
    for (auto u : us.List())
      qe.Set(u, dg.Edges(u).size() * dg.InvertedEdges(u).size());
    us.Clear();
  }
  return best_cost;
}
}  // namespace scycle
}  // namespace graph
