#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <queue>
#include <utility>
#include <vector>

template <template <class TData> class THeap, class TGraph,
          class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_HP(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  std::vector<unsigned> visited(graph.Size(), 0);
  THeap<TPair> q;
  vd[source] = TEdgeCost();
  for (q.Add({TEdgeCost(), source}); !q.Empty();) {
    auto p = q.Extract();
    unsigned u = p.second;
    if (!visited[u]) {
      visited[u] = true;
      for (auto e : graph.EdgesEI(u)) {
        unsigned v = e.to;
        TEdgeCost cost = p.first + f(e.info);
        if (cost < vd[v]) {
          vd[v] = cost;
          q.Add({cost, v});
        }
      }
    }
  }
  return vd;
}

template <class TUKeyPosMap, class TGraph, class TEdgeCostFunction,
          class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_KPM(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  TUKeyPosMap q(graph.Size());
  vd[source] = TEdgeCost();
  for (q.AddNewKey(source, TEdgeCost()); !q.Empty();) {
    auto p = q.Extract();
    unsigned u = p.key;
    for (auto e : graph.EdgesEI(u)) {
      unsigned v = e.to;
      TEdgeCost cost = p.value + f(e.info);
      if (cost < vd[v]) {
        vd[v] = cost;
        q.DecreaseValue(v, cost);
      }
    }
  }
  return vd;
}

template <class TUKeyValueMap, class TGraph, class TEdgeCostFunction,
          class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_KVM(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  TUKeyValueMap q(std::vector<TEdgeCost>(graph.Size(), max_cost), true);
  for (q.AddNewKey(source, TEdgeCost()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    TEdgeCost ucost = q.Get(u);
    for (auto e : graph.EdgesEI(u))
      q.DecreaseValueIfLess(e.to, ucost + f(e.info));
  }
  return q.GetValues();
}
