#pragma once

#include "common/node.h"

namespace graph {
namespace dynamic {
template <class TTData>
class Edge : public BaseNode {
 public:
  using TData = TTData;
  using TSelf = Edge<TData>;

  unsigned from;
  unsigned to;
  unsigned from_index;
  TSelf* reversed_edge;
  TTData data;
};
}  // namespace dynamic
}  // namespace graph
