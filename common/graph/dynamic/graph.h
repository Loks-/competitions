#pragma once

#include "common/base.h"
#include "common/graph/dynamic/edge.h"
#include "common/memory/nodes_manager.h"

#include <vector>

namespace graph {
namespace dynamic {
template <class TTEdgeData>
class Graph {
 public:
  using TEdgeData = TTEdgeData;
  using TEdge = Edge<TEdgeData>;
  using TManager = memory::NodesManager<TEdge>;
  using TSelf = Graph<TEdgeData>;

 protected:
  unsigned nvertices;
  TManager manager;
  std::vector<std::vector<TEdge*>> edges;

 public:
  void Clear() {
    nvertices = 0;
    manager.ResetNodes();
    edges.clear();
  }

  void Resize(unsigned _nvertices) {
    Clear();
    nvertices = _nvertices;
    edges.resize(nvertices);
  }

  explicit Graph(unsigned _nvertices = 0) { Resize(_nvertices); }
  unsigned Size() const { return nvertices; }

  std::vector<std::vector<TEdge*>>& Edges() { return edges; }

  const std::vector<std::vector<TEdge*>>& Edges() const { return edges; }

  std::vector<TEdge*>& Edges(unsigned from) { return edges[from]; }

  const std::vector<TEdge*>& Edges(unsigned from) const { return edges[from]; }

  unsigned EdgesSize() const {
    size_t total = 0;
    for (unsigned i = 0; i < nvertices; ++i) total += edges[i].size();
    return total / 2;
  }

 protected:
  void AddEdgeI(TEdge* edge, bool inverted) {
    auto& v = edges[inverted ? edge->u2 : edge->u1];
    (inverted ? edge->index_u2 : edge->index_u1) = v.size();
    v.push_back(edge);
  }

  void AddEdgeI(TEdge* edge) {
    AddEdgeI(edge, false);
    AddEdgeI(edge, true);
  }

 public:
  TEdge* AddEdge(unsigned u1, unsigned u2, const TEdgeData& data) {
    auto e = manager.New();
    e->u1 = u1;
    e->u2 = u2;
    e->data = data;
    AddEdgeI(e);
    return e;
  }

  TEdge* AddEdge(unsigned u1, unsigned u2) {
    assert(u1 != u2);
    return AddEdge(u1, u2, {});
  }

 protected:
  void DeleteEdgeI(TEdge* edge, bool inverted) {
    auto u = (inverted ? edge->u2 : edge->u1);
    auto index = (inverted ? edge->index_u2 : edge->index_u1);
    auto& v = edges[u];
    if (index + 1 != v.size()) {
      auto e2 = v.back();
      ((e2->u1 == u) ? e2->index_u1 : e2->index_u2) = index;
      v[index] = e2;
    }
    v.pop_back();
  }

  void DeleteEdgeI(TEdge* edge) {
    DeleteEdgeI(edge, false);
    DeleteEdgeI(edge, true);
  }

 public:
  void DeleteEdge(TEdge* edge) {
    DeleteEdgeI(edge);
    manager.Release(edge);
  }

  void MoveEdge(TEdge* edge, unsigned new_u1, unsigned new_u2) {
    DeleteEdgeI(edge);
    edge->u1 = new_u1;
    edge->u2 = new_u2;
    AddEdgeI(edge);
  }
};
}  // namespace dynamic
}  // namespace graph
