#pragma once

#include "tester/graph_type.h"
#include "tester/tester_graph_ei_distance_base_positive.h"

#include <string>

class TesterGraphEIDistanceUnsigned
    : public TesterGraphEIDistanceBasePositive<unsigned, true> {
 public:
  TesterGraphEIDistanceUnsigned(EGraphType _gtype, unsigned graph_size,
                                unsigned edges_per_node,
                                unsigned _max_edge_cost);

 protected:
  void TestDial();

 public:
  bool TestAll();
};
