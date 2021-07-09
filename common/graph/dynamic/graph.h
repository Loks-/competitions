#pragma once

#include "common/graph/dynamic/edge.h"
#include "common/nodes_manager.h"

#include <vector>

namespace graph {
namespace dynamic {
template <class TTEdgeData, bool _directed_edges = false>
class Graph {
 public:
  static const bool directed_edges = _directed_edges;
  using TEdgeData = TTEdgeData;
  using TEdge = Edge<TEdgeData>;
  using TManager = NodesManager<TEdge>;
  using TSelf = Graph<TEdgeData, directed_edges>;

 protected:
  unsigned nvertices;
  TManager manager;
  std::vector<std::vector<TEdge*>> edges;
  std::vector<std::vector<TEdge*>> inverted_edges;

 public:
  void Clear() {
    nvertices = 0;
    manager.ResetNodes();
    edges.clear();
    inverted_edges.clear();
  }

  void Resize(unsigned _nvertices) {
    Clear();
    nvertices = _nvertices;
    edges.resize(nvertices);
    if (directed_edges) inverted_edges.resize(nvertices);
  }

  explicit Graph(unsigned _nvertices = 0) { Resize(_nvertices); }
  unsigned Size() const { return nvertices; }

  std::vector<std::vector<TEdge*>>& Edges() { return edges; }

  const std::vector<std::vector<TEdge*>>& Edges() const { return edges; }

  std::vector<TEdge*>& Edges(unsigned from) { return edges[from]; }

  const std::vector<TEdge*>& Edges(unsigned from) const {
    return edges[from];
  }

  std::vector<std::vector<TEdge*>>& InvertedEdges() { return inverted_edges; }

  const std::vector<std::vector<TEdge*>>& InvertedEdges() const {
    return inverted_edges;
  }

  std::vector<TEdge*>& InvertedEdges(unsigned from) {
    return inverted_edges[from];
  }

  const std::vector<TEdge*>& InvertedEdges(unsigned from) const {
    return inverted_edges[from];
  }

  unsigned EdgesSize() const {
    size_t total = 0;
    for (unsigned i = 0; i < nvertices; ++i) total += edges[i].size();
    return (directed_edges ? total : total / 2);
  }

 protected:
  void AddEdgeI(TEdge* edge, bool inverted) {
    auto& v = (inverted ? inverted_edges : edges)[edge->from];
    edge->from_index = v.size();
    v.push_back(edge);
  }

 public:
  TEdge* AddEdge(unsigned from, unsigned to, const TEdgeData& data) {
    auto e1 = manager.New(), e2 = manager.New();
    e1->from = from;
    e1->to = to;
    e1->reversed_edge = e2;
    e1->data = data;
    e2->from = to;
    e2->to = from;
    e2->reversed_edge = e1;
    e2->data = data;
    AddEdgeI(e1, false);
    AddEdgeI(e2, directed_edges);
    return e1;
  }

  TEdge* AddEdge(unsigned from, unsigned to) {
    return AddEdge(from, to, {});
  }

 protected:
  void DeleteEdgeI(TEdge* edge, bool inverted) {
    auto& v = (inverted ? inverted_edges : edges)[edge->from];
    if (edge->from_index + 1 != v.size()) {
      v.back()->from_index = edge->from_index;
      v[edge->from_index] = v.back();
    }
    v.pop_back();
  }

 public:
  void DeleteEdge(TEdge* edge) {
    DeleteEdgeI(edge, false);
    DeleteEdgeI(edge->reversed_edge, directed_edges);
    manager.Release(edge->reversed_edge);
    manager.Release(edge);
  }
};
}  // namespace dynamic
}  // namespace graph
