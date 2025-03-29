#pragma once

#include "common/base.h"
#include "common/graph/dynamic/edge.h"
#include "common/memory/nodes_manager.h"

#include <vector>

namespace graph {
namespace dynamic {
template <class TTEdgeData, bool _directed_edges = false>
class Graph {
 public:
  static const bool directed_edges = _directed_edges;

  using TEdgeData = TTEdgeData;
  using TEdge = Edge<TEdgeData>;
  using TManager = memory::NodesManager<TEdge>;
  using TSelf = Graph<TEdgeData>;

 protected:
  unsigned nvertices;
  TManager manager;
  std::vector<std::vector<TEdge*>> edges;
  std::vector<std::vector<TEdge*>> inverted_edges;

 public:
  constexpr void Clear() {
    nvertices = 0;
    manager.clear();
    edges.clear();
    inverted_edges.clear();
  }

  constexpr void Resize(unsigned _nvertices) {
    Clear();
    nvertices = _nvertices;
    edges.resize(nvertices);
    if (directed_edges) inverted_edges.resize(nvertices);
  }

  constexpr explicit Graph(unsigned _nvertices = 0) { Resize(_nvertices); }

  constexpr unsigned Size() const { return nvertices; }

  constexpr std::vector<std::vector<TEdge*>>& Edges() { return edges; }

  constexpr const std::vector<std::vector<TEdge*>>& Edges() const {
    return edges;
  }

  constexpr std::vector<TEdge*>& Edges(unsigned from) { return edges[from]; }

  constexpr const std::vector<TEdge*>& Edges(unsigned from) const {
    return edges[from];
  }

  constexpr std::vector<std::vector<TEdge*>>& InvertedEdges() {
    return inverted_edges;
  }

  constexpr const std::vector<std::vector<TEdge*>>& InvertedEdges() const {
    return inverted_edges;
  }

  constexpr std::vector<TEdge*>& InvertedEdges(unsigned from) {
    return inverted_edges[from];
  }

  constexpr const std::vector<TEdge*>& InvertedEdges(unsigned from) const {
    return inverted_edges[from];
  }

  constexpr unsigned EdgesSize() const {
    size_t total = 0;
    for (unsigned i = 0; i < nvertices; ++i) total += edges[i].size();
    return (directed_edges ? total : total / 2);
  }

 protected:
  constexpr void AddEdgeI(TEdge* edge, bool inverted) {
    auto& v =
        inverted ? (directed_edges ? inverted_edges[edge->u2] : edges[edge->u2])
                 : edges[edge->u1];
    (inverted ? edge->index_u2 : edge->index_u1) = v.size();
    v.push_back(edge);
  }

  constexpr void AddEdgeI(TEdge* edge) {
    AddEdgeI(edge, false);
    AddEdgeI(edge, true);
  }

 public:
  constexpr TEdge* AddEdge(unsigned u1, unsigned u2, const TEdgeData& data) {
    auto e = manager.create();
    e->u1 = u1;
    e->u2 = u2;
    e->data = data;
    AddEdgeI(e);
    return e;
  }

  constexpr TEdge* AddEdge(unsigned u1, unsigned u2) {
    assert(directed_edges || (u1 != u2));
    return AddEdge(u1, u2, {});
  }

 protected:
  constexpr void DeleteEdgeI(TEdge* edge, bool inverted) {
    const auto u = (inverted ? edge->u2 : edge->u1);
    const auto index = (inverted ? edge->index_u2 : edge->index_u1);
    auto& v = ((inverted && directed_edges) ? inverted_edges : edges)[u];
    if (index + 1 != v.size()) {
      auto e2 = v.back();
      ((e2->u1 == u) ? e2->index_u1 : e2->index_u2) = index;
      v[index] = e2;
    }
    v.pop_back();
  }

  constexpr void DeleteEdgeI(TEdge* edge) {
    DeleteEdgeI(edge, false);
    DeleteEdgeI(edge, true);
  }

 public:
  constexpr void DeleteEdge(TEdge* edge) {
    DeleteEdgeI(edge);
    manager.release(edge);
  }

  constexpr void MoveEdge(TEdge* edge, unsigned new_u1, unsigned new_u2) {
    DeleteEdgeI(edge);
    edge->u1 = new_u1;
    edge->u2 = new_u2;
    AddEdgeI(edge);
  }
};

template <class TTEdgeData>
using UndirectedGraph = Graph<TTEdgeData, false>;

template <class TTEdgeData>
using DirectedGraph = Graph<TTEdgeData, true>;
}  // namespace dynamic
}  // namespace graph
