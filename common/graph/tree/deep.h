#pragma once

#include "common/graph/graph/distance.h"
#include "common/graph/tree.h"

template <class TTBaseTree>
inline std::vector<unsigned> TreeDeep(const TTBaseTree& tree) {
  return DistanceFromSource(tree, tree.GetRoot());
}
