#pragma once

#include "common/base.h"
#include "common/data_structures/disjoint_set.h"
#include "common/graph/tree.h"
#include "common/graph/tree/convert_to_tree_graph.h"
#include "common/graph/tree_ei.h"

#include <vector>

// TODO:
//   Check speed if we use linear algorithm instead of disjoint set.
namespace graph {
namespace tpm {
template <class TEdgeInfo, class TEdgeCostFunction>
inline void FullBranchingTree(
    const TreeEI<TEdgeInfo>& tree, const TEdgeCostFunction& f,
    TreeGraph& output_fbt,
    std::vector<typename TEdgeCostFunction::TEdgeCost>& output_nodes) {
  using TEdgeCost = typename TEdgeCostFunction::TEdgeCost;
  class TEdge1 {
   public:
    unsigned u;
    TEdgeCost cost;
  };
  class TEdge2 {
   public:
    unsigned u0, u1;
    TEdgeCost cost;
  };

  unsigned n = tree.Size(), n2 = 2 * n - 1;
  assert(n > 0);
  output_nodes.resize(0);
  output_nodes.resize(n2, TEdgeCost());
  if (n == 1) {
    output_fbt = ConvertToTreeGraph(tree);
    return;
  }

  output_fbt.Resize(n2);
  ds::DisjointSet tds(n2);
  std::vector<TEdge2> edges, edges2;
  std::vector<TEdge1> best_edge(n2, {CNone, TEdgeCost()});
  std::vector<unsigned> umap(n2, CNone);
  unsigned k2 = n;
  for (unsigned i = 0; i < n; ++i) {
    for (const auto& e : tree.EdgesEI(i)) {
      if (i < e.to) edges.push_back({i, e.to, f(e.info)});
    }
  }
  for (unsigned k0 = 0, k1 = n; k1 > k0 + 1;) {
    for (auto& e : edges) {
      if ((best_edge[e.u0].u == CNone) || (e.cost < best_edge[e.u0].cost))
        best_edge[e.u0] = {e.u1, e.cost};
    }
    for (auto& e : edges) {
      if ((best_edge[e.u1].u == CNone) || (e.cost < best_edge[e.u1].cost))
        best_edge[e.u1] = {e.u0, e.cost};
    }
    for (unsigned u = k0; u < k1; ++u) {
      assert(best_edge[u].u != CNone);
      output_nodes[u] = best_edge[u].cost;
      tds.Union(u, best_edge[u].u);
    }
    for (unsigned u = k0; u < k1; ++u) {
      if (tds.Find(u) == u) umap[u] = k2++;
    }
    for (unsigned u = k0; u < k1; ++u) {
      umap[u] = umap[tds.Find(u)];
      output_fbt.AddEdge(u, umap[u]);
    }
    edges2.clear();
    for (auto& e : edges) {
      if (umap[e.u0] != umap[e.u1])
        edges2.push_back({umap[e.u0], umap[e.u1], e.cost});
    }
    edges.swap(edges2);
    k0 = k1;
    k1 = k2;
  }
  assert(edges.empty());
  output_fbt.ResizeWithoutClear(k2);
  output_fbt.SetRoot(k2 - 1);
  output_nodes.resize(k2);
}
}  // namespace tpm
}  // namespace graph
