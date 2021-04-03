#pragma once

#include "common/graph/tree.h"

#include <utility>
#include <vector>

namespace graph {
namespace lca {
// Tarjan algorithm for offline LCA
// Time: O(V) preprocessing, O(1) lca
template <class TLCA, class TGraph>
inline std::vector<unsigned> OfflineProxy(
    const Tree<TGraph>& g,
    const std::vector<std::pair<unsigned, unsigned>>& q) {
  TLCA lca(g);
  std::vector<unsigned> output;
  output.reserve(q.size());
  for (auto p : q) output.push_back(lca.GetLCA(p.first, p.second));
  return output;
}
}  // namespace lca
}  // namespace graph
