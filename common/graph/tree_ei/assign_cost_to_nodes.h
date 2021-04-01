#pragma once

#include "common/graph/tree_ei.h"

#include <stack>
#include <utility>
#include <vector>

namespace graph {
template <class TEdgeInfo, class TEdgeCostFunction>
inline std::vector<TEdgeInfo> AssignCostToNodes(const TreeEI<TEdgeInfo>& tree, const TEdgeCostFunction& f) {
  std::vector<TEdgeInfo> output(tree.Size(), TEdgeInfo());
  std::stack<std::pair<unsigned, unsigned>> s;
  for (s.push(std::make_pair(tree.GetRoot(), CNone)); !s.empty();) {
    unsigned v = s.top().first, p = s.top().second;
    s.pop();
    assert(tree.Edges(v).size() == tree.EdgesInfo(v).size());
    for (unsigned i = 0; i < tree.Edges(v).size(); ++i) {
      unsigned u = tree.Edges(v)[i];
      if (u == p) continue;
      output[u] = f(tree.EdgesInfo(v)[i]);
      s.push(std::make_pair(u, v));
    }
  }
  return output;
}
}  // namespace graph
