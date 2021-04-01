#pragma once

#include "common/base.h"
#include "common/graph/tree.h"
#include "common/graph/tree/hld.h"
#include "common/segment_tree/info/max.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace graph {
namespace tpm {
template<class TGraph, class TValue>
inline std::vector<TValue> TPM_HLD(const Tree<TGraph>& tree, const std::vector<TValue>& nodes_values, const std::vector<std::pair<unsigned, unsigned>>& paths, bool ignore_lca = false) {
  assert(tree.Size() == nodes_values.size());
  std::vector<TValue> output;
  graph::HLD<TValue, st::info::Max<TValue>> hld(tree);
  for (unsigned i = 0; i < tree.Size(); ++i) hld.SetData(i, nodes_values[i]);
  for (auto& p : paths) {
    if (ignore_lca) {
      unsigned lca = hld.LCA(p.first, p.second);
      output.push_back(std::max(hld.PathFromAncestor(lca, p.first, true).GetInfo().max, hld.PathFromAncestor(lca, p.second, true).GetInfo().max));
    } else {
      output.push_back(hld.Path(p.first, p.second).GetInfo().max);
    }
  }
  return output;
}
}  // namespace tpm
}  // namespace graph
