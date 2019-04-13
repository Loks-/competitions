#pragma once

#include "common/graph/graph/distance.h"
#include "common/graph/tree.h"

namespace graph {
template <class TTree>
inline std::vector<unsigned> TreeDeep(const TTree& tree) {
  return DistanceFromSource(tree, tree.GetRoot());
}
}  // namespace graph
