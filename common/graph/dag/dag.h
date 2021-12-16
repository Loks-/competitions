#pragma once

#include "common/graph/dag/topological_sort.h"
#include "common/graph/graph.h"

inline bool IsDAG(const DirectedGraph& g) {
  return TopologicalSort(g).size() == g.Size();
}
