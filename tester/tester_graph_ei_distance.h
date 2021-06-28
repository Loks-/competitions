#pragma once

#include "tester/graph_type.h"
#include "tester/tester_graph_ei_distance_base.h"

#include "common/base.h"

class TesterGraphEIDistance : public TesterGraphEIDistanceBase<int64_t, true> {
 public:
  TesterGraphEIDistance(EGraphType _gtype, unsigned graph_size,
                        unsigned edges_per_node);

  bool TestAll();
};
