#pragma once

namespace graph {
template <class TTEdgeInfo>
class EdgeCostProxy {
 public:
  using TEdgeInfo = TTEdgeInfo;
  using TEdgeCost = TTEdgeInfo;
  const TEdgeCost& operator()(const TEdgeInfo& edge) const { return edge; }
};
}  // namespace graph
