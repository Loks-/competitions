#pragma once

#include "common/graph/edge.h"
#include "common/graph/graph.h"

#include <iostream>
#include <vector>

namespace graph {
template <class TTEdgeInfo, bool _directed_edges = false>
class GraphEI : public Graph<_directed_edges> {
 public:
  static const bool directed_edges = _directed_edges;
  using TEdgeInfo = TTEdgeInfo;
  using TEdge = Edge<TEdgeInfo>;
  using TEdges = EdgesFromVertex<TEdgeInfo>;
  using TBase = Graph<directed_edges>;
  using TSelf = GraphEI<TEdgeInfo, directed_edges>;

 protected:
  std::vector<std::vector<TEdgeInfo>> edges_info;
  std::vector<std::vector<TEdgeInfo>> inverted_edges_info;

 public:
  void Clear() {
    TBase::Clear();
    edges_info.clear();
    inverted_edges_info.clear();
  }

  void ClearEdges(unsigned from) {
    TBase::edges[from].clear();
    edges_info[from].clear();
  }

  void Resize(unsigned nvertices) {
    Clear();
    TBase::Resize(nvertices);
    edges_info.resize(nvertices);
    if (directed_edges) inverted_edges_info.resize(nvertices);
  }

  explicit GraphEI(unsigned _nvertices = 0) { Resize(_nvertices); }

  TEdges EdgesEI(unsigned from) const {
    return TEdges(TBase::edges[from], edges_info[from]);
  }

  TEdges InvertedEdgesEI(unsigned from) const {
    return TEdges(TBase::inverted_edges[from], inverted_edges_info[from]);
  }

  std::vector<std::vector<TEdgeInfo>>& EdgesInfo() { return edges_info; }

  const std::vector<std::vector<TEdgeInfo>>& EdgesInfo() const {
    return edges_info;
  }

  std::vector<TEdgeInfo>& EdgesInfo(unsigned from) { return edges_info[from]; }

  const std::vector<TEdgeInfo>& EdgesInfo(unsigned from) const {
    return edges_info[from];
  }

  std::vector<std::vector<TEdgeInfo>>& InvertedEdgesInfo() {
    return inverted_edges_info;
  }

  const std::vector<std::vector<TEdgeInfo>>& InvertedEdgesInfo() const {
    return inverted_edges_info;
  }

  std::vector<TEdgeInfo>& InvertedEdgesInfo(unsigned from) {
    return inverted_edges_info[from];
  }

  const std::vector<TEdgeInfo>& InvertedEdgesInfo(unsigned from) const {
    return inverted_edges_info[from];
  }

  void AddBaseEdge(unsigned from, unsigned to, const TEdgeInfo& edge_info) {
    TBase::AddBaseEdge(from, to);
    edges_info[from].push_back(edge_info);
  }

  void AddInvertedEdge(unsigned from, unsigned to, const TEdgeInfo& edge_info) {
    TBase::AddInvertedEdge(from, to);
    inverted_edges_info[from].push_back(edge_info);
  }

  void AddEdge(unsigned from, unsigned to, const TEdgeInfo& edge_info) {
    AddBaseEdge(from, to, edge_info);
    if (directed_edges)
      AddInvertedEdge(to, from, edge_info);
    else
      AddBaseEdge(to, from, edge_info);
  }

  void ReadEdges(unsigned edges_to_read, bool zero_based_indexes = false) {
    unsigned shift = zero_based_indexes ? 0 : 1;
    for (; edges_to_read--;) {
      unsigned from, to;
      TEdgeInfo edge_info;
      std::cin >> from >> to >> edge_info;
      AddEdge(from - shift, to - shift, edge_info);
    }
  }
};
}  // namespace graph

template <class TEdgeInfo>
using UndirectedGraphEI = graph::GraphEI<TEdgeInfo, false>;
template <class TEdgeInfo>
using DirectedGraphEI = graph::GraphEI<TEdgeInfo, true>;
