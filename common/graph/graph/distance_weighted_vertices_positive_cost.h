#pragma once

#include "common/graph/graph.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance_positive_cost.h"

#include <vector>

// Distance with weighted cost to visit each vertex (except starting one).
template <class TWeight, bool directed_edges>
inline std::vector<TWeight> DistanceFromSourceWeightedVerticesPositiveCost(
    const graph::Graph<directed_edges>& g, const std::vector<TWeight>& w,
    unsigned source, const TWeight& max_cost) {
  graph::GraphEI<TWeight, true> gnew(g.Size());
  for (unsigned u = 0; u < g.Size(); ++u) {
    for (auto v : g.Edges(u)) gnew.AddEdge(u, v, w[v]);
  }
  return DistanceFromSourcePositiveCost(gnew, source, max_cost);
}
