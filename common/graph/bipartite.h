#pragma once

#include "common/base.h"
#include "common/graph/graph.h"

#include <algorithm>
#include <vector>

namespace graph {
template <class TGraph>
class Bipartite : public TGraph {
 public:
  using TBase = TGraph;
  using TSelf = Bipartite<TGraph>;

 protected:
  unsigned nvertices0, nvertices1;
  std::vector<bool> partition;

 public:
  constexpr void Clear() {
    TBase::Clear();
    nvertices0 = nvertices1 = 0;
    partition.clear();
  }

  constexpr void Resize(unsigned _nvertices0, unsigned _nvertices1) {
    TBase::Resize(_nvertices0 + _nvertices1);
    nvertices0 = _nvertices0;
    nvertices1 = _nvertices1;
    partition.resize(nvertices0 + nvertices1);
    std::fill(partition.begin(), partition.begin() + nvertices0, false);
    std::fill(partition.begin() + nvertices0, partition.end(), true);
  }

  constexpr void Resize(const std::vector<bool>& _partition) {
    TBase::Resize(_partition.size());
    partition = _partition;
    nvertices0 = nvertices1 = 1;
    for (bool b : partition) {
      nvertices0 += (b ? 0 : 1);
      nvertices1 += (b ? 1 : 0);
    }
  }

  constexpr Bipartite() { Resize(0, 0); }

  constexpr Bipartite(unsigned _nvertices0, unsigned _nvertices1) {
    Resize(_nvertices0, _nvertices1);
  }

  constexpr explicit Bipartite(const std::vector<bool>& _partition) {
    Resize(_partition);
  }

  constexpr bool Partition(unsigned node) const { return partition[node]; }

  constexpr const std::vector<bool>& Partition() const { return partition; };

  constexpr unsigned Partition0Size() const { return nvertices0; }

  constexpr unsigned Partition1Size() const { return nvertices1; }

  constexpr void AddBaseEdge(unsigned from, unsigned to) {
    assert(partition[from] != partition[to]);
    TBase::AddBaseEdge(from, to);
  }

  constexpr void AddInvertedEdge(unsigned from, unsigned to) {
    assert(partition[from] != partition[to]);
    TBase::AddInvertedEdge(from, to);
  }

  constexpr void AddEdge(unsigned from, unsigned to) {
    assert(partition[from] != partition[to]);
    TBase::AddEdge(from, to);
  }
};
}  // namespace graph

using BipartiteGraph = graph::Bipartite<UndirectedGraph>;
