#pragma once

#include "common/graph/dynamic/connected_components_count.h"
#include "common/graph/dynamic/graph.h"
#include "common/template.h"

namespace graph {
namespace dynamic {
namespace connectivity {
// Slow version that run DFS on each request
// It should be used only for testing.
class Base {
 public:
  using TGraph = graph::dynamic::UndirectedGraph<MetaEmpty>;
  using TEdge = typename TGraph::TEdge;
  using TEdgeID = TEdge*;

 protected:
  TGraph g;

 public:
  Base(unsigned size) : g(size) {}

  TEdgeID InsertEdge(unsigned u1, unsigned u2) { return g.AddEdge(u1, u2); }

  void RemoveEdge(TEdgeID edge) { g.DeleteEdge(edge); }

  unsigned Components() const { return ConnectedComponentsCount(g); }
};
}  // namespace connectivity
}  // namespace dynamic
}  // namespace graph
