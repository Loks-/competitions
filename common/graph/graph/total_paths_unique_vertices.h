#pragma once

#include "common/base.h"
#include "common/graph/graph.h"

#include <unordered_map>

namespace graph {
template <class TGraph, class TValue = uint64_t>
class TotalPathsUniqueVerticesS52 {
 protected:
  TGraph g;
  std::unordered_map<uint64_t, TValue> cache;

 protected:
  TValue CountI(unsigned b, unsigned e, uint64_t mask) {
    if (b == e) return TValue(1);
    uint64_t key = b + (e << 6) + (mask << 12);
    auto it = cache.find(key);
    if (it != cache.end()) return it->second;
    TValue s(0);
    mask &= ~(1ull << b);
    for (auto u : g.Edges(b)) {
      if (mask & (1ull << u)) s += CountI(u, e, mask);
    }
    return cache[key] = s;
  }

 public:
  TotalPathsUniqueVerticesS52(const TGraph& g_) : g(g_) {
    assert(g.Size() <= 52);
  }

  TValue Count(unsigned b, unsigned e, uint64_t mask) {
    if (((1ull << b) | (1ull << e)) & ~mask) return TValue(0);
    return CountI(b, e, mask);
  }
};
}  // namespace graph

template <bool directed_edges, class TValue = uint64_t>
inline TValue TotalPathsUniqueVertices(const graph::Graph<directed_edges>& g,
                                       unsigned b, unsigned e) {
  graph::TotalPathsUniqueVerticesS52<graph::Graph<directed_edges>, TValue> tpuv(
      g);
  return tpuv.Count(b, e, (1ull << g.Size()) - 1);
}

template <bool directed_edges, class TValue = uint64_t>
inline TValue TotalPathsUniqueVertices(const graph::Graph<directed_edges>& g,
                                       unsigned b, unsigned e, uint64_t mask) {
  graph::TotalPathsUniqueVerticesS52<graph::Graph<directed_edges>, TValue> tpuv(
      g);
  return tpuv.Count(b, e, mask);
}
