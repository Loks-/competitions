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
  constexpr void Clear() {
    nvertices = 0;
    edges.clear();
  }

  constexpr void Resize(unsigned _nvertices) {
    Clear();
    nvertices = _nvertices;
    edges.resize(nvertices);
  }

  constexpr explicit Graph(unsigned _nvertices = 0) { Resize(_nvertices); }

  constexpr Graph(unsigned _nvertices, unsigned _source, unsigned _sink)
      : source(_source), sink(_sink) {
    Resize(_nvertices);
  }

  constexpr void SetSourceSink(unsigned _source, unsigned _sink) {
    ResetFlow();
    source = _source;
    sink = _sink;
  }

  constexpr unsigned Size() const { return nvertices; }

  constexpr unsigned Source() const { return source; }

  constexpr unsigned Sink() const { return sink; }

  constexpr std::vector<std::vector<TEdge>>& Edges() { return edges; }

  constexpr const std::vector<std::vector<TEdge>>& Edges() const {
    return edges;
  }

  constexpr std::vector<TEdge>& Edges(unsigned from) { return edges[from]; }

  constexpr const std::vector<TEdge>& Edges(unsigned from) const {
    return edges[from];
  }

  constexpr void AddDataEdge(const TData& data, unsigned from, unsigned to,
                             TFlow max_flow, TFlow max_flow_inverted = 0) {
    edges[from].push_back(
        {data, max_flow, 0, from, to, unsigned(edges[to].size())});
    edges[to].push_back({-data, max_flow_inverted, 0, to, from,
                         unsigned(edges[from].size() - 1)});
  }

  constexpr void AddEdge(unsigned from, unsigned to, TFlow max_flow,
                         TFlow max_flow_inverted = 0) {
    AddDataEdge(TData(), from, to, max_flow, max_flow_inverted);
  }

  constexpr TEdge& ReversedEdge(const TEdge& e) {
    return edges[e.to][e.reversed_edge_index];
  }

  constexpr const TEdge& ReversedEdge(const TEdge& e) const {
    return edges[e.to][e.reversed_edge_index];
  }

  constexpr void ResetFlow() {
    for (unsigned i = 0; i < nvertices; ++i) {
      for (auto& e : Edges(i)) e.flow = 0;
    }
  }

  constexpr TFlow Flow() const {
    TFlow f = 0;
    for (auto& e : Edges(source)) f += e.flow;
    return f;
  }
};
}  // namespace flow
}  // namespace graph

template <class TEdge>
using FlowGraph = graph::flow::Graph<TEdge>;
