#pragma once

#include "common/base.h"
#include "common/graph/dynamic/graph.h"
#include "common/graph/graph_ei.h"

namespace graph {
namespace dynamic {
// If graph is undirected self edges are not supported and ignored.
template <class TEdgeInfo, bool directed_edges>
void Convert(const graph::GraphEI<TEdgeInfo, directed_edges>& g,
             graph::dynamic::Graph<TEdgeInfo, directed_edges>& output) {
  output.Resize(g.Size());
  for (unsigned u = 0; u < g.Size(); ++u) {
    auto& ue = g.Edges(u);
    auto& uei = g.EdgesInfo(u);
    assert(ue.size() == uei.size());
    for (unsigned j = 0; j < ue.size(); ++j) {
      auto v = ue[j];
      if (directed_edges || (u < v)) output.AddEdge(u, v, uei[j]);
    }
  }
}
}  // namespace dynamic
}  // namespace graph
