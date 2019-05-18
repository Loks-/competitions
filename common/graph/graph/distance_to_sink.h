#pragma once

#include "common/graph/graph.h"
#include <queue>
#include <vector>

// Return distance (as number of edges) from each vertex to required vertex.
// If from some vertex sink is unreachable it returns -1 for it.
template <bool directed_edges>
inline std::vector<unsigned> DistanceToSink(
    const graph::Graph<directed_edges>& g, unsigned sink) {
  const unsigned none = -1;
  std::vector<unsigned> d(g.Size(), none);
  d[sink] = 0;
  std::queue<unsigned> q;
  q.push(sink);
  while (!q.empty()) {
    unsigned f = q.front();
    q.pop();
    unsigned l = d[f];
    for (unsigned k : g.Edges(f)) {
      if (d[k] == none) {
        d[k] = l + 1;
        q.push(k);
      }
    }
  }
  return d;
}
