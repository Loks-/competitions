#pragma once

#include "common/graph/graph.h"
#include "common/permutation/inverse_line.h"
#include <algorithm>

// Time: O(V + E log V)
inline void SortEdgesByOrder(UndirectedGraph& g, std::vector<unsigned>& order,
                             bool inversed_order = false) {
  auto order_pos = permutation::Inverse(order);
  auto cmp = [&](unsigned i, unsigned j) {
    return inversed_order != (order_pos[i] < order_pos[j]);
  };
  for (unsigned i = 0; i < g.Size(); ++i)
    std::sort(g.Edges(i).begin(), g.Edges(i).end(), cmp);
}
