#pragma once

#include "common/base.h"
#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/hash.h"
#include "common/hash/vector.h"

#include <algorithm>
#include <functional>
#include <unordered_set>
#include <utility>
#include <vector>

namespace graph {
class HamiltonianPathUndirectedGraph {
 protected:
  UndirectedGraph g;
  unsigned size;
  std::vector<unsigned> current_path;
  std::vector<unsigned> visited;
  std::unordered_set<size_t> cache;

 protected:
  size_t Hash() const {
    DHash<std::vector<unsigned>> h;
    return HashCombine(h(visited), current_path.back());
  }

  bool Connected(unsigned from) const {
    auto v = DistanceFromSource(g, from);
    for (unsigned d : v) {
      if (d > size) return false;
    }
    return true;
  }

  bool FindI() {
    if (current_path.size() == size) return true;
    size_t h = Hash();
    if (cache.find(h) != cache.end()) return false;
    cache.insert(h);
    unsigned current = current_path.back();
    std::vector<std::pair<unsigned, unsigned>> vp;
    for (unsigned u : g.Edges(current)) {
      if (visited[u]) continue;
      unsigned k = 0;
      for (unsigned v : g.Edges(u)) {
        if (!visited[v]) ++k;
      }
      vp.push_back({k, u});
    }
    std::sort(vp.begin(), vp.end());
    if (vp.size() == 0) return false;
    if ((vp[0].first == 0) && (current_path.size() + 1 < size)) return false;
    for (auto p : vp) {
      unsigned u = p.second;
      visited[u] = 1;
      current_path.push_back(u);
      if (FindI()) return true;
      current_path.pop_back();
      visited[u] = 0;
    }
    return false;
  }

 public:
  void ResetCache() { cache.clear(); }
  std::vector<unsigned> GetPath() const { return current_path; }

  bool Find(unsigned from, bool check_connection = true) {
    if (check_connection && !Connected(from)) return false;
    current_path.clear();
    std::fill(visited.begin(), visited.end(), 0);
    current_path.push_back(from);
    visited[from] = 1;
    return FindI();
  }

  bool Find() {
    if (!g.directed_edges && !Connected(0)) return false;
    for (unsigned i = 0; i < size; ++i) {
      if (Find(i, g.directed_edges)) return true;
    }
    return false;
  }

  explicit HamiltonianPathUndirectedGraph(const UndirectedGraph& g)
      : g(g), size(g.Size()) {
    current_path.reserve(size);
    visited.resize(size, 0);
  }
};
}  // namespace graph

inline std::vector<unsigned> HamiltonianPath(const UndirectedGraph& g) {
  graph::HamiltonianPathUndirectedGraph hp(g);
  return hp.Find() ? hp.GetPath() : std::vector<unsigned>();
}
