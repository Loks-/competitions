#pragma once

#include "common/graph/graph.h"
#include "common/graph/graph/articulation_points/dfs_stack.h"

#include <vector>

inline std::vector<unsigned> ArticulationPoints(const UndirectedGraph& g) {
  return graph::ap::DFSStack(g);
}
