#pragma once

#include "common/graph/base_graph/distance.h"
#include "common/graph/base_tree/base_tree.h"

template <class TTBaseTree>
inline std::vector<unsigned> GetDeep(const TTBaseTree& tree) {
  return GetEdgeDistancesFromSource(tree, tree.GetRoot());
}
