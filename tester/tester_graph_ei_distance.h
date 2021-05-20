#pragma once

#include "tester/graph_type.h"

#include "common/base.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <string>
#include <vector>

class TesterGraphEIDistance {
 public:
  using TGraph = graph::GraphEI<int64_t, true>;
  using TEdgeCostFunction = graph::EdgeCostProxy<int64_t>;

 protected:
  EGraphType gtype;
  TGraph g;
  TEdgeCostFunction edge_proxy;

 public:
  TesterGraphEIDistance(EGraphType _gtype, unsigned graph_size,
                                    unsigned edges_per_node);

 protected:
  size_t TestBellmanFord() const;
  size_t TestFloydWarshall() const;
  size_t TestSPFA() const;
  size_t TestSPFALLL() const;
  size_t TestSPFASLF() const;

 public:
  bool TestAll();
};
