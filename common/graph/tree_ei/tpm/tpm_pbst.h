#pragma once

#include "common/base.h"
#include "common/binary_search_tree/persistent/treap.h"
#include "common/binary_search_tree/subtree_data/max.h"
#include "common/binary_search_tree/subtree_data/utils/get_by_key.h"
#include "common/graph/tree.h"
#include "common/graph/tree/lca.h"

#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

namespace graph {
namespace tpm {
template <class TGraph, class TValue>
// Solve tree path maxima problem using LCA and Persistent Binary Search Tree.
inline std::vector<TValue> TPM_PBST(
    const Tree<TGraph>& tree, const std::vector<TValue>& nodes_values,
    const std::vector<std::pair<unsigned, unsigned>>& paths,
    bool ignore_lca = false) {
  using TMax = bst::subtree_data::Max<TValue>;
  using TTree = bst::persistent::Treap<true, false, TValue, std::tuple<TMax>>;
  using TNode = typename TTree::TNode;

  assert(tree.Size() == nodes_values.size());
  std::vector<TValue> output;
  graph::LCA lca(tree);
  TTree ptree;
  std::vector<TNode*> roots(tree.Size());
  std::stack<std::pair<unsigned, unsigned>> s;
  for (s.push(std::make_pair(tree.GetRoot(), CNone)); !s.empty();) {
    unsigned v = s.top().first, p = s.top().second;
    s.pop();
    roots[v] = ptree.insert_new((p == CNone) ? nullptr : roots[p],
                                nodes_values[v], lca.deep[v]);
    for (auto u : tree.Edges(v)) {
      if (u == p) continue;
      s.push(std::make_pair(u, v));
    }
  }

  for (auto& p : paths) {
    if ((p.first == p.second) && ignore_lca) {
      output.push_back(0);
    } else {
      unsigned a = lca.GetLCA(p.first, p.second);
      output.push_back(
          std::max(TMax::get(bst::subtree_data::get_by_key(
                       roots[p.first], lca.deep[a] + (ignore_lca ? 1 : 0),
                       lca.deep[p.first] + 1)),
                   TMax::get(bst::subtree_data::get_by_key(
                       roots[p.second], lca.deep[a] + (ignore_lca ? 1 : 0),
                       lca.deep[p.second] + 1))));
    }
  }
  return output;
}
}  // namespace tpm
}  // namespace graph
