#pragma once

#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/tree_ei.h"
#include "common/graph/tree_ei/tpm/tpm_pbst_fbt.h"

#include <utility>
#include <vector>

namespace graph {
// Cost on edges
template <class TEdgeInfo, class TEdgeCostFunction>
inline std::vector<typename TEdgeCostFunction::TEdgeCost> TreePathMaxima(
    const TreeEI<TEdgeInfo>& tree, const TEdgeCostFunction& f,
    const std::vector<std::pair<unsigned, unsigned>>& paths) {
  return tpm::TPM_PBST_FBT(tree, f, paths);
}

template <class TEdgeInfo>
inline std::vector<TEdgeInfo> TreePathMaxima(
    const TreeEI<TEdgeInfo>& tree,
    const std::vector<std::pair<unsigned, unsigned>>& paths) {
  return TreePathMaxima(tree, EdgeCostProxy<TEdgeInfo>{}, param_type);
}
}  // namespace graph
