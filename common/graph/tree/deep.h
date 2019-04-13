#pragma once

#include "common/graph/graph/distance.h"
#include "common/graph/tree.h"

template <class TTBaseTree>
inline std::vector<unsigned> GetDeep(const TTBaseTree& tree) {
  return GetEdgeDistancesFromSource(tree, tree.GetRoot());
}
