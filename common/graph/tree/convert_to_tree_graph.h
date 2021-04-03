#pragma once

#include "common/graph/tree.h"

namespace graph {
template <class TGraph>
inline TreeGraph ConvertToTreeGraph(const Tree<TGraph>& input) {
  TreeGraph output(input.Size());
  output.SetRoot(input.GetRoot());
  for (unsigned u = 0; u < input.Size(); ++u) {
    for (unsigned v : input.Edges(u)) {
      output.AddBaseEdge(u, v);
    }
  }
  return output;
}
}  // namespace graph
