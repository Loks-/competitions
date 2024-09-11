#pragma once

#include "common/graph/graph.h"

#include <iostream>
#include <vector>

namespace graph {
template <class TGraph>
class Tree : public TGraph {
 public:
  using TBase = TGraph;
  using TSelf = Tree;

 protected:
  unsigned root;

 public:
  constexpr explicit Tree(unsigned _nvertices = 0, unsigned _root = 0)
      : TBase(_nvertices), root(_root) {}

  constexpr unsigned GetRoot() const { return root; }

  constexpr void SetRoot(unsigned new_root) { root = new_root; }

  constexpr void Resize(unsigned _nvertices, unsigned _root = 0) {
    TBase::Resize(_nvertices);
    root = _root;
  }

  constexpr unsigned EdgesSize() const { return TBase::Size() - 1; }

  constexpr void ReadEdges(bool zero_based_indexes = false) {
    TBase::ReadEdges(TBase::Size() - 1, zero_based_indexes);
  }

  constexpr void ReadTreeEdges(bool zero_based_indexes = false) {
    const unsigned shift = zero_based_indexes ? 0 : 1;
    for (unsigned i = 1; i < TBase::nvertices; ++i) {
      unsigned from;
      std::cin >> from;
      TBase::AddEdge(from - shift, i);
    }
  }
};
}  // namespace graph

using TreeGraph = graph::Tree<UndirectedGraph>;
