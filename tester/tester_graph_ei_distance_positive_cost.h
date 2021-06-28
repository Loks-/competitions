#pragma once

#include "tester/graph_type.h"
#include "tester/tester_graph_ei_distance_base_positive.h"

#include "common/base.h"

class TesterGraphEIDistancePositiveCost
    : public TesterGraphEIDistanceBasePositive<uint64_t, true> {
 public:
  TesterGraphEIDistancePositiveCost(EGraphType _gtype, unsigned graph_size,
                                    unsigned edges_per_node);

  bool TestAll();
};
