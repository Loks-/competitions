#pragma once

#include "common/graph/graph_ei.h"
#include "common/graph/tree.h"

#include <iostream>
#include <vector>

namespace graph {
template <class TTEdgeInfo>
class TreeEI : public Tree<UndirectedGraphEI<TTEdgeInfo>> {
 public:
  using TTree = Tree<UndirectedGraphEI<TTEdgeInfo>>;
  using TGraph = typename TTree::TBase;
  using TBaseGraph = typename TGraph::TBase;
  using TEdgeInfo = typename TGraph::TEdgeInfo;
  using TSelf = TreeEI<TTEdgeInfo>;

 public:
  constexpr explicit TreeEI(unsigned _nvertices = 0, unsigned _root = 0)
      : TTree(_nvertices, _root) {}

  constexpr void ReadEdges(bool zero_based_indexes = false) {
    TGraph::ReadEdges(TBaseGraph::nvertices - 1, zero_based_indexes);
  }

  void ReadTreeEdges(bool zero_based_indexes = false) {
    const unsigned shift = zero_based_indexes ? 0 : 1;
    for (unsigned i = 1; i < TBaseGraph::nvertices; ++i) {
      TEdgeInfo edge_info;
      unsigned from;
      std::cin >> from >> edge_info;
      TGraph::AddEdge(from - shift, i, edge_info);
    }
  }
};
}  // namespace graph

template <class TEdgeInfo>
using TreeGraphEI = graph::TreeEI<TEdgeInfo>;
