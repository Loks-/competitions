#pragma once

#include "common/base.h"
#include "common/graph/cnone.h"

#include <iostream>
#include <vector>

namespace graph {
template <bool _directed_edges = false>
class Graph {
 public:
  static const bool directed_edges = _directed_edges;
  using TSelf = Graph<directed_edges>;

 protected:
  unsigned nvertices;
  std::vector<std::vector<unsigned>> edges;
  std::vector<std::vector<unsigned>> inverted_edges;

 public:
  void Clear() {
    nvertices = 0;
    edges.clear();
    inverted_edges.clear();
  }

  void Resize(unsigned _nvertices) {
    Clear();
    nvertices = _nvertices;
    edges.resize(nvertices);
    if (directed_edges) inverted_edges.resize(nvertices);
  }

  void ResizeWithoutClear(unsigned _nvertices) {
    for (unsigned i = _nvertices; i < nvertices; ++i) {
      assert((edges[i].size() == 0) &&
             (!directed_edges || (inverted_edges[i].size() == 0)));
    }
    nvertices = _nvertices;
    edges.resize(nvertices);
    if (directed_edges) inverted_edges.resize(nvertices);
  }

  explicit Graph(unsigned _nvertices = 0) { Resize(_nvertices); }
  unsigned Size() const { return nvertices; }

  std::vector<std::vector<unsigned>>& Edges() { return edges; }

  const std::vector<std::vector<unsigned>>& Edges() const { return edges; }

  std::vector<unsigned>& Edges(unsigned from) { return edges[from]; }

  const std::vector<unsigned>& Edges(unsigned from) const {
    return edges[from];
  }

  std::vector<std::vector<unsigned>>& InvertedEdges() { return inverted_edges; }

  const std::vector<std::vector<unsigned>>& InvertedEdges() const {
    return inverted_edges;
  }

  std::vector<unsigned>& InvertedEdges(unsigned from) {
    return inverted_edges[from];
  }

  const std::vector<unsigned>& InvertedEdges(unsigned from) const {
    return inverted_edges[from];
  }

  unsigned EdgesSize() const {
    size_t total = 0;
    for (unsigned i = 0; i < nvertices; ++i) total += edges[i].size();
    return (directed_edges ? total : total / 2);
  }

  void AddBaseEdge(unsigned from, unsigned to) { edges[from].push_back(to); }

  void AddInvertedEdge(unsigned from, unsigned to) {
    inverted_edges[from].push_back(to);
  }

  void AddEdge(unsigned from, unsigned to) {
    AddBaseEdge(from, to);
    if (directed_edges)
      AddInvertedEdge(to, from);
    else
      AddBaseEdge(to, from);
  }

  void ReadEdges(unsigned edges_to_read, bool zero_based_indexes = false) {
    unsigned shift = zero_based_indexes ? 0 : 1;
    for (; edges_to_read--;) {
      unsigned from, to;
      std::cin >> from >> to;
      AddEdge(from - shift, to - shift);
    }
  }
};
}  // namespace graph

using UndirectedGraph = graph::Graph<false>;
using DirectedGraph = graph::Graph<true>;
