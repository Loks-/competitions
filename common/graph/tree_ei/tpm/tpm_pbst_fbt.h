#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/segment/get_by_key.h"
#include "common/binary_search_tree/info/segment/max.h"
#include "common/binary_search_tree/info/segment/none.h"
#include "common/binary_search_tree/persistent/treap.h"
#include "common/graph/tree.h"
#include "common/graph/tree/lca.h"
#include "common/graph/tree_ei.h"
#include "common/graph/tree_ei/tpm/full_branching_tree.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace graph {
namespace tpm {
template <class TEdgeInfo, class TEdgeCostFunction>
// Solve tree path maxima problem using LCA and Persistent Binary Search Tree.
inline std::vector<typename TEdgeCostFunction::TEdgeCost> TPM_PBST_FBT(
    const TreeEI<TEdgeInfo>& tree, const TEdgeCostFunction& f,
    const std::vector<std::pair<unsigned, unsigned>>& paths) {
  using TValue = typename TEdgeCostFunction::TEdgeCost;
  using TTree = bst::persistent::Treap<
      true, false, TValue,
      bst::info::segment::Max<TValue, bst::info::segment::None>>;
  using TNode = typename TTree::TNode;

  if (tree.Size() <= 1) return std::vector<TValue>(paths.size(), TValue());

  TreeGraph fbt;
  std::vector<TValue> nodes_values;
  FullBranchingTree(tree, f, fbt, nodes_values);
  assert(fbt.Size() == nodes_values.size());
  unsigned s = fbt.Size();
  std::vector<TValue> output;
  graph::LCA lca(fbt);
  TTree ptree;
  std::vector<TNode*> roots(s, nullptr);
  for (unsigned i = s - 1; i > 0; --i)
    roots[i - 1] = ptree.InsertNewNode(roots[fbt.Edges(i - 1).back()],
                                       nodes_values[i - 1], lca.deep[i - 1]);

  unsigned d = lca.deep[0] + 1;
  for (auto& p : paths) {
    if (p.first == p.second) {
      output.push_back(0);
    } else {
      unsigned a = lca.GetLCA(p.first, p.second), da = lca.deep[a] + 1;
      output.push_back(
          std::max(bst::info::segment::GetByKey(roots[p.first], da, d).max,
                   bst::info::segment::GetByKey(roots[p.second], da, d).max));
    }
  }
  return output;
}
}  // namespace tpm
}  // namespace graph
