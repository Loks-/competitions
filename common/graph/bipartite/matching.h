#pragma once

#include "common/graph/bipartite.h"

#include <queue>
#include <vector>

namespace graph {
// Hopcroft-Karp algorithm
// https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
// Time: O(E sqrt(V))
class BipartiteMatching {
 protected:
  const unsigned inf = unsigned(-1);
  unsigned size, matched;
  std::vector<unsigned> vpair;
  std::vector<unsigned> vdist;
  std::queue<unsigned> q;

 protected:
  template <class TGraph>
  bool BFS(const Bipartite<TGraph>& graph) {
    vdist[size] = inf;
    for (unsigned u = 0; u < size; ++u) {
      if (graph.Partition(u) || (vpair[u] != size)) {
        vdist[u] = inf;
      } else {
        vdist[u] = 0;
        q.push(u);
      }
    }
    for (; !q.empty(); q.pop()) {
      unsigned u = q.front();
      if (vdist[u] < vdist[size]) {
        for (unsigned v : graph.Edges(u)) {
          unsigned vp = vpair[v];
          if (vdist[vp] == inf) {
            vdist[vp] = vdist[u] + 1;
            q.push(vp);
          }
        }
      }
    }
    return vdist[size] != inf;
  }

  template <class TGraph>
  bool DFS(const Bipartite<TGraph>& graph, unsigned u) {
    if (u == size) return true;
    for (unsigned v : graph.Edges(u)) {
      unsigned vp = vpair[v];
      if (vdist[vp] == vdist[u] + 1) {
        if (DFS(graph, vp)) {
          vpair[v] = u;
          vpair[u] = v;
          return true;
        }
      }
    }
    vdist[u] = inf;
    return false;
  }

 public:
  template <class TGraph>
  unsigned Matching(const Bipartite<TGraph>& graph) {
    size = graph.Size();
    vpair.clear();
    vpair.resize(size, size);
    vdist.resize(size + 1);
    matched = 0;
    for (; BFS(graph);) {
      for (unsigned u = 0; u < size; ++u) {
        if (graph.Partition(u)) continue;
        if (vpair[u] == size) {
          if (DFS(graph, u)) ++matched;
        }
      }
    }
    return matched;
  }
};
}  // namespace graph

template <class TGraph>
inline unsigned BipartiteMatching(const graph::Bipartite<TGraph>& graph) {
  graph::BipartiteMatching bm;
  return bm.Matching(graph);
}
