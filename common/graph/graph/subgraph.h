#pragma once

#include "common/base.h"
#include "common/graph/graph.h"
#include "common/graph/tree.h"

#include <vector>

namespace graph {
template <class TGraph>
inline void SubgraphUpdateRootI(const TGraph&, unsigned,
                                const std::vector<unsigned>&, TGraph&) {}

inline void SubgraphUpdateRootI(const TreeGraph& g, unsigned new_nvertices,
                                const std::vector<unsigned>& old_to_new,
                                TreeGraph& output) {
  unsigned root_new = old_to_new[g.GetRoot()];
  output.SetRoot((root_new >= new_nvertices) ? 0 : root_new);
}

template <class TGraph>
inline TGraph Subgraph(const TGraph& g, unsigned new_nvertices,
                       const std::vector<unsigned>& old_to_new) {
  assert(g.Size() == old_to_new.size());
  TGraph output(new_nvertices);
  for (unsigned u = 0; u < g.Size(); ++u) {
    unsigned unew = old_to_new[u];
    if (unew >= new_nvertices) continue;
    for (unsigned v : g.Edges(u)) {
      unsigned vnew = old_to_new[v];
      if (vnew >= new_nvertices) continue;
      output.AddBaseEdge(unew, vnew);
    }
    if (g.directed_edges) {
      for (unsigned v : g.InvertedEdges(u)) {
        unsigned vnew = old_to_new[v];
        if (vnew >= new_nvertices) continue;
        output.AddInvertedEdge(unew, vnew);
      }
    }
  }
  SubgraphUpdateRootI(g, new_nvertices, old_to_new, output);
  return output;
}

template <class TGraph>
inline TGraph Subgraph(const TGraph& g, const std::vector<bool>& keep) {
  assert(g.Size() == keep.size());
  unsigned new_nvertices = 0;
  std::vector<unsigned> old_to_new(g.Size(), CNone);
  for (unsigned i = 0; i < keep.size(); ++i) {
    if (keep[i]) old_to_new[i] = new_nvertices++;
  }
  return Subgraph(g, new_nvertices, old_to_new);
}
}  // namespace graph
