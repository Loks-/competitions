#pragma once

#include "common/base.h"
#include "common/graph/flow/empty.h"

namespace graph {
namespace flow {
template <class TTData = Empty, class TTFlow = int64_t>
class Edge {
 public:
  using TData = TTData;
  using TFlow = TTFlow;

  TTData data;
  TFlow max_flow;
  TFlow flow;
  unsigned from;
  unsigned to;
  unsigned reversed_edge_index;
};
}  // namespace flow
}  // namespace graph

using FlowGraphEdge = graph::flow::Edge<>;
