#pragma once

#include "common/base.h"
#include "common/template.h"

namespace graph {
namespace flow {
template <class TTData = TEmpty, class TTFlow = int64_t>
class Edge {
 public:
  using TData = TTData;
  using TFlow = TTFlow;

  TTData data;
  TFlow max_flow;
  TFlow flow;
  unsigned to;
  unsigned reverse_edge_index;
};
}  // namespace flow
}  // namespace graph

using FlowGraphEdge = graph::flow::Edge<>;
