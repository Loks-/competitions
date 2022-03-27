#pragma once

#include "common/graph/graph.h"
#include "common/graph/graph/strongly_connected_components/tarjan.h"

#include <vector>

inline std::vector<unsigned> StronglyConnectedComponents(
    const DirectedGraph& graph) {
  return graph::scc::Tarjan(graph);
}
