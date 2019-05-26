#pragma once

#include "common/graph/graph.h"
#include "common/permutation/invert_line.h"
#include <algorithm>

inline void SortEdgesByOrder(UndirectedGraph& g, std::vector<unsigned>& order,
                             bool inversed_order = false) {
  auto order_pos = InvertLine(order);
  auto cmp = [&](unsigned i, unsigned j) {
    return inversed_order != (order_pos[i] < order_pos[j]);
  };
  for (unsigned i = 0; i < g.Size(); ++i)
    std::sort(g.Edges(i).begin(), g.Edges(i).end(), cmp);
}
