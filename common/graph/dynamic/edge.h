#pragma once

#include "common/memory/node.h"

namespace graph {
namespace dynamic {
template <class TTData>
class Edge : public memory::Node {
 public:
  using TData = TTData;
  using TSelf = Edge<TData>;

 public:
  unsigned u1, u2;
  unsigned index_u1, index_u2;
  TTData data;

 public:
  constexpr unsigned Other(unsigned u0) const { return u1 + u2 - u0; }
};
}  // namespace dynamic
}  // namespace graph
