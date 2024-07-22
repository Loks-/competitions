#pragma once

#include "common/base.h"
#include "common/data_structures/unsigned_set.h"
#include "common/graph/dynamic/convert.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/graph_ei.h"
#include "common/heap/ukvm/dheap.h"

#include <algorithm>

// Code in this file was not tested.

namespace graph {
namespace scycle {
// Find shortest cycle length by running dijkstra from each node.
// All edges cost are non-negative.
// Time: O(V (V + E) log V)
namespace hidden {
template <class TValue, class TUKVMHeap>
void DijkstraUpdateI(const graph::dynamic::DirectedGraph<TValue>& g,
                     unsigned source, TValue& best_cost, const TValue& max_cost,
                     TUKVMHeap& q, ds::UnsignedSet& us) {
  // Run Dijkstra
  us.Insert(source);
  for (q.AddNewKey(source, TValue()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    auto ucost = q.Get(u);
    if (!(ucost < best_cost)) continue;
    for (auto e : g.EdgesEI(u)) {
      auto ecost = ucost + e.data;
      if (!(ecost < best_cost)) continue;
      if (e.u2 == source) {
        best_cost = ecost;
        continue;
      }
      us.Insert(e.u2);
      q.DecreaseValueIfLess(e.u2, ecost);
    }
  }
  // Restore heap and set
  for (auto u : us.List()) q.AddNewKey(u, max_cost, true);
  us.Clear();
}
}  // namespace hidden

template <class TValue>
TValue Dijkstra(const DirectedGraphEI<TValue>& g, TValue max_cost) {
  TValue best_cost = max_cost;
  heap::ukvm::DHeap<4u, TValue> qd(std::vector<TValue>(g.Size(), max_cost),
                                   true);
  ds::UnsignedSet us(g.Size());
  graph::dynamic::DirectedGraph<TValue> dg;
  graph::dynamic::Convert(g, dg);
  std::vector<unsigned> vpc(g.Size());
  for (unsigned i = 0; i < vpc.size(); ++i)
    vpc[i] = dg.Edges(i).size() * dg.InvertedEdges(i).size();
  heap::ukvm::DHeap<4u, unsigned, std::greater<unsigned>> qe(vpc, false);
  for (; !qe.Empty();) {
    auto u = qe.ExtractKey();
    if (qe.Get(u) == 0) break;
    hidden::DijkstraUpdateI(dg, u, best_cost, max_cost, qd, us);
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
