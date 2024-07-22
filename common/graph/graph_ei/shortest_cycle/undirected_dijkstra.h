#pragma once

#include "common/base.h"
#include "common/data_structures/unsigned_set.h"
#include "common/graph/dynamic/convert.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/graph_ei.h"
#include "common/heap/ukvm/dheap.h"

#include <algorithm>
#include <vector>

namespace graph {
namespace scycle {
// Find shortest cycle length by running dijkstra from each node.
// All edges cost are non-negative.
// Time: O(V (V + E) log V)
namespace hidden {
template <class TValue, class TUKVMHeap>
void DijkstraUpdateI(const graph::dynamic::UndirectedGraph<TValue>& g,
                     unsigned source, TValue& best_cost, const TValue& max_cost,
                     TUKVMHeap& q, ds::UnsignedSet& us,
                     std::vector<unsigned>& vf) {
  // Run Dijkstra
  us.Insert(source);
  vf[source] = unsigned(-1);
  for (q.AddNewKey(source, TValue()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    auto ucost = q.Get(u);
    if (!(ucost + ucost < best_cost)) {
      q.ExtractAll();
      break;
    }
    for (auto e : g.Edges(u)) {
      auto v = e->Other(u);
      if (v == vf[u]) continue;
      auto ecost = ucost + e->data;
      if (!(ucost + ecost < best_cost)) continue;
      if (q.InHeap(v)) best_cost = std::min(best_cost, q.Get(v) + ecost);
      if (ecost < q.Get(v)) {
        us.Insert(v);
        q.Set(v, ecost);
        vf[v] = u;
      }
    }
  }
  // Restore heap and set
  for (auto u : us.List()) q.AddNewKey(u, max_cost, true);
  us.Clear();
}
}  // namespace hidden

template <class TValue>
TValue Dijkstra(const UndirectedGraphEI<TValue>& g, TValue max_cost) {
  TValue best_cost = max_cost;
  for (unsigned u = 0; u < g.Size(); ++u) {
    for (auto e : g.EdgesEI(u)) {
      if (e.to == u) best_cost = std::min(best_cost, e.info);
    }
  }
  heap::ukvm::DHeap<4u, TValue> qd(std::vector<TValue>(g.Size(), max_cost),
                                   true);
  ds::UnsignedSet us(g.Size());
  std::vector<unsigned> vf(g.Size());
  graph::dynamic::UndirectedGraph<TValue> dg;
  graph::dynamic::Convert(g, dg);
  std::vector<unsigned> vpc(g.Size());
  for (unsigned i = 0; i < vpc.size(); ++i) vpc[i] = dg.Edges(i).size();
  heap::ukvm::DHeap<4u, unsigned, std::greater<unsigned>> qe(vpc, false);
  for (; !qe.Empty();) {
    auto u = qe.ExtractKey();
    if (qe.Get(u) == 0) break;
    hidden::DijkstraUpdateI(dg, u, best_cost, max_cost, qd, us, vf);
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
