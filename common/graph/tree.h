#pragma once

#include "common/graph/graph.h"
#include <iostream>
#include <vector>

template <class TGraph>
class Tree : public TGraph {
 public:
  using TBase = TGraph;
  using TSelf = Tree;

 protected:
  unsigned root;

 public:
  Tree(unsigned _nvertices = 0, unsigned _root = 0)
      : TBase(_nvertices), root(_root) {}
  unsigned GetRoot() const { return root; }
  void SetRoot(unsigned new_root) { root = new_root; }

  void Resize(unsigned _nvertices, unsigned _root = 0) {
    TBase::Resize(_nvertices);
    root = _root;
  }

  void ReadEdges(bool zero_based_indexes = false) {
    TBase::ReadEdges(TBase::nvertices - 1, zero_based_indexes);
  }

  void ReadTreeEdges(bool zero_based_indexes = false) {
    unsigned shift = zero_based_indexes ? 0 : 1;
    for (unsigned i = 1; i < TBase::nvertices; ++i) {
      unsigned from;
      std::cin >> from;
      TBase::AddEdge(from - shift, i);
    }
  }
};

using TTree = Tree<UndirectedGraph>;
