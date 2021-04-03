#pragma once

#include "common/data_structures/disjoint_set.h"
#include "common/graph/tree.h"

#include <functional>
#include <utility>
#include <vector>

namespace graph {
namespace lca {
// Tarjan algorithm for offline LCA
// Time: O(g.Size() + q.Size())
template <class TGraph>
inline std::vector<unsigned> TarjanOffline(
    const Tree<TGraph>& g,
    const std::vector<std::pair<unsigned, unsigned>>& q) {
  unsigned n = g.Size();
  std::vector<unsigned> output(q.size());
  std::vector<std::vector<std::pair<unsigned, unsigned>>> q2(n);
  for (unsigned i = 0; i < q.size(); ++i) {
    q2[q[i].first].push_back({q[i].second, i});
    q2[q[i].second].push_back({q[i].first, i});
  }
  std::vector<unsigned> va(n, CNone);
  ds::DisjointSet djs(n);

  std::function<void(unsigned)> dfs = [&](unsigned u) -> void {
    va[u] = u;
    for (auto v : g.Edges(u)) {
      if (va[v] == CNone) {
        dfs(v);
        djs.Union(v, u);
        va[djs.Find(u)] = u;
      }
    }
    for (auto p : q2[u]) {
      if (va[p.first] != CNone) {
        output[p.second] = va[djs.Find(p.first)];
      }
    }
  };

  dfs(g.GetRoot());
  return output;
}
}  // namespace lca
}  // namespace graph
