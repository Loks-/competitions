#pragma once

#include "common/base.h"
#include "common/graph/base_graph/base_graph.h"
#include "common/graph/base_tree/base_tree.h"
#include <vector>

class BaseSubgraph {
 protected:
  template <class TBaseGraph>
  static void UpdateRoot(const TBaseGraph& graph, unsigned new_nvertices,
                         const std::vector<unsigned>& old_to_new,
                         TBaseGraph& output) {}
  static void UpdateRoot(const TBaseTree& graph, unsigned new_nvertices,
                         const std::vector<unsigned>& old_to_new,
                         TBaseTree& output) {
    unsigned root_new = old_to_new[graph.GetRoot()];
    output.SetRoot((root_new >= new_nvertices) ? 0 : root_new);
  }

 public:
  template <class TBaseGraph>
  static void CreateSubgraph(const TBaseGraph& graph, unsigned new_nvertices,
                             const std::vector<unsigned>& old_to_new,
                             TBaseGraph& output) {
    assert(graph.Size() == old_to_new.size());
    output.Resize(new_nvertices);
    for (unsigned u = 0; u < graph.Size(); ++u) {
      unsigned unew = old_to_new[u];
      if (unew >= new_nvertices) continue;
      for (unsigned v : graph.Edges(u)) {
        unsigned vnew = old_to_new[v];
        if (vnew >= new_nvertices) continue;
        output.AddBaseEdge(unew, vnew);
      }
      if (graph.directed_edges) {
        for (unsigned v : graph.InvertedEdges(u)) {
          unsigned vnew = old_to_new[v];
          if (vnew >= new_nvertices) continue;
          output.AddInvertedEdge(unew, vnew);
        }
      }
    }
    UpdateRoot(graph, new_nvertices, old_to_new, output);
  }

  template <class TBaseGraph>
  static void CreateSubgraph(const TBaseGraph& graph,
                             const std::vector<bool>& keep,
                             TBaseGraph& output) {
    assert(graph.Size() == keep.size());
    unsigned new_nvertices = 0;
    std::vector<unsigned> old_to_new(graph.Size(), CNone);
    for (unsigned i = 0; i < keep.size(); ++i) {
      if (keep[i]) old_to_new[i] = new_nvertices++;
    }
    CreateSubgraph(graph, new_nvertices, old_to_new, output);
  }
};
