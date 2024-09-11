#pragma once

#include "common/base.h"
#include "common/graph/graph.h"
#include "common/graph/graph/chain_decomposition.h"
#include "common/template.h"

#include <vector>

namespace graph {
namespace ap {
// Time: O(V + E)
// Current realization works only for connected graphs.
inline std::vector<unsigned> ChainDecomposition(const UndirectedGraph& g) {
  graph::ChainDecomposition cd;
  const bool b = cd.Build(g);
  assert(b);
  MetaUse(b);
  return cd.ArticulationPoints();
}
}  // namespace ap
}  // namespace graph
