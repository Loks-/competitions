#include "tester/tester_graph_ei_distance.h"

#include "tester/graph_type.h"

#include "common/graph/graph_ei/replace_edge_info.h"
#include "common/vector/hrandom.h"

#include <iostream>

TesterGraphEIDistance::TesterGraphEIDistance(EGraphType _gtype,
                                             unsigned graph_size,
                                             unsigned edges_per_node)
    : TesterGraphEIDistanceBase(_gtype, graph_size, edges_per_node, (1u << 30),
                                (1ll << 60)) {
  auto vp = nvector::HRandom<uint64_t>(graph_size, 0, (1u << 30));
  auto f = [&](unsigned from, unsigned to, int64_t cost) {
    return int64_t((vp[from] << 10) - (vp[to] << 10) + cost);
  };
  g = ReplaceEdgeInfo<int64_t, int64_t>(g, f);
}

bool TesterGraphEIDistance::TestAll() {
  PrintGraphType();
  TestSPFA();
  TestAllPairs();
  return CheckHash();
}

bool TestGraphEIDistance(bool time_test) {
  if (time_test) {
    TesterGraphEIDistance t1(EGraphType::SPARSE, 5000, 4),
        t2(EGraphType::DENSE, 1000, 400), t3(EGraphType::LONG_PATH, 300, 500);
    return t1.TestAll() && t2.TestAll() && t3.TestAll();
  } else {
    TesterGraphEIDistance t(EGraphType::SMALL, 10, 4);
    return t.TestAll();
  }
}
