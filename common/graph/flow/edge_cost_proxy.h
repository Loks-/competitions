#pragma once

namespace graph {
namespace flow {
template <class TTEdge>
class EdgeCostProxy {
 public:
  using TEdge = TTEdge;
  using TCost = typename TTEdge::TData;
  const TCost& operator()(const TEdge& edge) const { return edge.data; }
};
}  // namespace flow
}  // namespace graph
