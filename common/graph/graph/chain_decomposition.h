#pragma once

#include "common/graph/graph.h"

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace graph {
// Current realization works only for connected graphs.
class ChainDecomposition {
 protected:
  std::vector<unsigned> articulation_points;
  std::vector<std::pair<unsigned, unsigned>> bridges;
  std::vector<std::vector<unsigned>> chains;

 public:
  // Output false if graph not connected.
  bool Build(const UndirectedGraph& g, unsigned root = 0) {
    Clear();
    const unsigned n = g.Size();
    std::vector<unsigned> visited(n, 0), parent(n, CNone), dfi, ap(n, 0);
    dfi.reserve(n);
    std::vector<std::vector<unsigned>> backedges(n);

    const std::function<void(unsigned)> DFS = [&](unsigned u) -> void {
      dfi.push_back(u);
      visited[u] = dfi.size();
      for (auto v : g.Edges(u)) {
        if (!visited[v]) {
          parent[v] = u;
          DFS(v);
        } else {
          if (visited[v] > visited[u]) backedges[u].push_back(v);
        }
      }
    };

    // Build chain decomposition
    DFS(root);
    if (dfi.size() != n) return false;
    std::fill(visited.begin(), visited.end(), 0u);
    for (auto u : dfi) {
      if (!visited[u]) {
        if (parent[u] != CNone) bridges.push_back({parent[u], u});
        visited[u] = 1;
      }
      for (auto v : backedges[u]) {
        std::vector<unsigned> c(1, u);
        auto w = v;
        for (; !visited[w]; w = parent[w]) {
          c.push_back(w);
          visited[w] = 1;
        }
        c.push_back(w);
        chains.push_back(c);
      }
    }

    // Construct articulation points
    for (unsigned i = 1; i < chains.size(); ++i) {
      const auto& c = chains[i];
      if (c[0] == c.back()) ap[c[0]] = 1;
    }
    for (auto& p : bridges) {
      for (auto u : {p.first, p.second}) {
        if (g.Edges(u).size() > 1) ap[u] = 1;
      }
    }
    for (unsigned u = 0; u < n; ++u) {
      if (ap[u]) articulation_points.push_back(u);
    }
    return true;
  }

  void Clear() {
    articulation_points.clear();
    bridges.clear();
    chains.clear();
  }

  const std::vector<unsigned>& ArticulationPoints() const {
    return articulation_points;
  }

  const std::vector<std::pair<unsigned, unsigned>>& Bridges() const {
    return bridges;
  }

  const std::vector<std::vector<unsigned>>& Chains() const { return chains; }
};
}  // namespace graph
