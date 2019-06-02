#pragma once

#include "common/base.h"
#include "common/graph/edge.h"
#include "common/graph/graph_ei.h"
#include "common/stl/hash/pair.h"
#include <unordered_map>
#include <utility>
#include <vector>

template <class TEdgeInfo>
// Time: O(V + E)
inline UndirectedGraphEI<TEdgeInfo> Dual(
    const UndirectedGraphEI<TEdgeInfo>& g) {
  std::vector<std::unordered_map<unsigned, unsigned>> node_to_index(g.Size());
  for (unsigned i = 0; i < g.Size(); ++i) {
    const auto& edges = g.Edges(i);
    for (unsigned j = 0; j < edges.size(); ++j) {
      node_to_index[i][edges[j]] = j;
    }
  }
  unsigned edges = g.EdgesSize();
  std::unordered_map<std::pair<unsigned, unsigned>, unsigned> edge_to_face;
  edge_to_face.reserve(2 * edges);
  unsigned current_face = 0;
  for (unsigned i = 0; i < g.Size(); ++i) {
    for (unsigned u : g.Edges(i)) {
      auto it = edge_to_face.find({i, u});
      if (it != edge_to_face.end()) continue;
      unsigned v0 = i, v1 = u;
      for (; it == edge_to_face.end(); it = edge_to_face.find({v0, v1})) {
        edge_to_face[{v0, v1}] = current_face;
        unsigned j = node_to_index[v1][v0];
        v0 = v1;
        unsigned l1 = g.Edges(v1).size();
        v1 = g.Edges(v1)[(j + 1) % l1];
      }
      ++current_face;
    }
  }
  assert(g.Size() + current_face == edges + 2);
  UndirectedGraphEI<TEdgeInfo> gd(current_face);
  for (unsigned i = 0; i < g.Size(); ++i) {
    for (const auto& e : g.EdgesEI(i)) {
      auto it1 = edge_to_face.find({i, e.to});
      auto it2 = edge_to_face.find({e.to, i});
      assert((it1 != edge_to_face.end()) && (it2 != edge_to_face.end()));
      gd.AddBaseEdge(it1->second, it2->second, e.info);
    }
  }
  return gd;
}
