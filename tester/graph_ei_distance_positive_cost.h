#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/heap/binary_heap.h"
#include "common/heap/ukey_value_heap.h"
#include "common/heap/ukey_value_map.h"

#include <queue>
#include <utility>
#include <vector>

template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_PQ(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  std::priority_queue<TPair, std::vector<TPair>, std::greater<TPair>> q;
  for (q.push({TEdgeCost(), source}); !q.empty();) {
    auto p = q.top();
    q.pop();
    unsigned u = p.second;
    if (vd[u] == max_cost) {
      vd[u] = p.first;
      for (auto e : graph.EdgesEI(u)) {
        unsigned v = e.to;
        if (vd[v] == max_cost) {
          TEdgeCost cost = p.first + f(e.info);
          if (cost < max_cost) q.push({cost, v});
        }
      }
    }
  }
  return vd;
}

template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_BH(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  heap::BinaryHeap<TPair> q;
  for (q.Add({TEdgeCost(), source}); !q.Empty();) {
    auto p = q.Extract();
    unsigned u = p.second;
    if (vd[u] == max_cost) {
      vd[u] = p.first;
      for (auto e : graph.EdgesEI(u)) {
        unsigned v = e.to;
        if (vd[v] == max_cost) {
          TEdgeCost cost = p.first + f(e.info);
          if (cost < max_cost) q.Add({cost, v});
        }
      }
    }
  }
  return vd;
}

template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_CBH(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  using TPair = std::pair<TEdgeCost, unsigned>;
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  std::vector<unsigned> visited(graph.Size(), 0);
  heap::BinaryHeap<TPair> q;
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

template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_KVH(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  std::vector<TEdgeCost> vd(graph.Size(), max_cost);
  heap::UKeyValueHeap<TEdgeCost> q(graph.Size());
  vd[source] = TEdgeCost();
  for (q.Set(source, TEdgeCost()); !q.Empty();) {
    auto p = q.Extract();
    unsigned u = p.key;
    for (auto e : graph.EdgesEI(u)) {
      unsigned v = e.to;
      TEdgeCost cost = p.value + f(e.info);
      if (cost < vd[v]) {
        vd[v] = cost;
        q.Set(v, cost);
      }
    }
  }
  return vd;
}

template <class TGraph, class TEdgeCostFunction, class TEdgeCost>
std::vector<TEdgeCost> DistanceFromSourcePositiveCost_KVM(
    const TGraph& graph, const TEdgeCostFunction& f, unsigned source,
    const TEdgeCost& max_cost) {
  heap::UKeyValueMap<TEdgeCost> q(
      std::vector<TEdgeCost>(graph.Size(), max_cost), true);
  for (q.Set(source, TEdgeCost()); !q.Empty();) {
    unsigned u = q.ExtractKey();
    TEdgeCost ucost = q.Get(u);
    for (auto e : graph.EdgesEI(u)) {
      unsigned v = e.to;
      TEdgeCost cost = ucost + f(e.info);
      if (cost < q.Get(v)) {
        q.Set(v, cost);
      }
    }
  }
  return q.GetValues();
}
