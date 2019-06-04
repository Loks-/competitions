#pragma once

#include "common/graph/flow/edge.h"
#include <vector>

namespace graph {
namespace flow {
template <class TTEdge>
class Graph {
 public:
  using TEdge = TTEdge;
  using TData = typename TEdge::TData;
  using TFlow = typename TEdge::TFlow;

 protected:
  unsigned nvertices;
  std::vector<std::vector<TEdge>> edges;

 public:
  void Clear() {
    nvertices = 0;
    edges.clear();
  }

  void Resize(unsigned _nvertices) {
    Clear();
    nvertices = _nvertices;
    edges.resize(nvertices);
  }

  Graph(unsigned _nvertices = 0) { Resize(_nvertices); }
  unsigned Size() const { return nvertices; }
  std::vector<std::vector<TEdge>>& Edges() { return edges; }
  const std::vector<std::vector<TEdge>>& Edges() const { return edges; }
  std::vector<TEdge>& Edges(unsigned from) { return edges[from]; }
  const std::vector<TEdge>& Edges(unsigned from) const { return edges[from]; }

  void AddEdge(const TData& data, unsigned from, unsigned to, TEdge max_flow,
               TEdge max_flow_inverted = 0) {
    edges[from].push_back({data, max_flow, 0, to, edges[to].size()});
    edges[to].push_back(
        {data, max_flow_inverted, 0, to, edges[from].size() - 1});
  }

  void AddEdge(unsigned from, unsigned to, TEdge max_flow,
               TEdge max_flow_inverted = 0) {
    AddEdge(TData(), from, to, max_flow, max_flow_inverted);
  }
};
}  // namespace flow
}  // namespace graph

template <class TEdge>
using FlowGraph = graph::flow::Graph<TEdge>;
