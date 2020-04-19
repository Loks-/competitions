#pragma once

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
  unsigned source;
  unsigned sink;
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

  explicit Graph(unsigned _nvertices = 0) { Resize(_nvertices); }
  Graph(unsigned _nvertices, unsigned _source, unsigned _sink)
      : source(_source), sink(_sink) {
    Resize(_nvertices);
  }

  void SetSourceSink(unsigned _source, unsigned _sink) {
    ResetFlow();
    source = _source;
    sink = _sink;
  }

  unsigned Size() const { return nvertices; }
  unsigned Source() const { return source; }
  unsigned Sink() const { return sink; }
  std::vector<std::vector<TEdge>>& Edges() { return edges; }
  const std::vector<std::vector<TEdge>>& Edges() const { return edges; }
  std::vector<TEdge>& Edges(unsigned from) { return edges[from]; }
  const std::vector<TEdge>& Edges(unsigned from) const { return edges[from]; }

  void AddDataEdge(const TData& data, unsigned from, unsigned to,
                   TFlow max_flow, TFlow max_flow_inverted = 0) {
    edges[from].push_back(
        {data, max_flow, 0, from, to, unsigned(edges[to].size())});
    edges[to].push_back({data, max_flow_inverted, 0, to, from,
                         unsigned(edges[from].size() - 1)});
  }

  void AddEdge(unsigned from, unsigned to, TFlow max_flow,
               TFlow max_flow_inverted = 0) {
    AddDataEdge(TData(), from, to, max_flow, max_flow_inverted);
  }

  TEdge& ReversedEdge(const TEdge& e) {
    return edges[e.to][e.reversed_edge_index];
  }

  const TEdge& ReversedEdge(const TEdge& e) const {
    return edges[e.to][e.reversed_edge_index];
  }

  void ResetFlow() {
    for (unsigned i = 0; i < nvertices; ++i) {
      for (auto& e : Edges(i)) e.flow = 0;
    }
  }

  TFlow Flow() const {
    TFlow f = 0;
    for (auto& e : Edges(source)) f += e.flow;
    return f;
  }
};
}  // namespace flow
}  // namespace graph

template <class TEdge>
using FlowGraph = graph::flow::Graph<TEdge>;
