#pragma once

#include "common/graph/base_graph.h"
#include "common/graph/edge.h"
#include <iostream>
#include <vector>

template <class TTEdgeInfo, bool _directed_edges = false>
class Graph : public BaseGraph<_directed_edges> {
 public:
  const static bool directed_edges = _directed_edges;
  using TEdgeInfo = TTEdgeInfo;
  using TGraphEdge = GraphEdge<TEdgeInfo>;
  using TGraphEdges = GraphEdgesFromVertex<TEdgeInfo>;
  using TBase = BaseGraph<directed_edges>;
  using TSelf = Graph<TEdgeInfo, directed_edges>;

 protected:
  unsigned nvertices;
  std::vector<std::vector<TEdgeInfo>> edges_info;
  std::vector<std::vector<TEdgeInfo>> inverted_edges_info;

 public:
  void Clear() {
    TBase::Clear();
    edges_info.clear();
    inverted_edges_info.clear();
  }

  void Resize(unsigned nvertices) {
    Clear();
    TBase::Resize(nvertices);
    edges_info.resize(nvertices);
    if (directed_edges) inverted_edges_info.resize(nvertices);
  }

  Graph(unsigned _nvertices = 0) { Resize(_nvertices); }

  TGraphEdges GraphEdges(unsigned from) const {
    return TGraphEdges(TBase::edges[from], edges_info[from]);
  }

  TGraphEdges InvertedGraphEdges(unsigned from) const {
    return TGraphEdges(TBase::inverted_edges[from], inverted_edges_info[from]);
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

template <class EdgeInfo>
using UndirectedGraph = Graph<EdgeInfo, false>;
template <class EdgeInfo>
using DirectedGraph = Graph<EdgeInfo, true>;
