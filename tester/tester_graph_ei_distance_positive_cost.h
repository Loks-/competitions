#pragma once

#include "common/base.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <string>
#include <vector>

class TesterGraphEIDistancePositiveCost {
 public:
  using TGraph = graph::GraphEI<uint64_t, true>;
  using TEdgeCostFunction = graph::EdgeCostProxy<uint64_t>;

 protected:
  TGraph g;
  TEdgeCostFunction edge_proxy;

 public:
  TesterGraphEIDistancePositiveCost(unsigned graph_size,
                                    unsigned edges_per_node);

 protected:
  template <template <class TData> class THeap>
  size_t TestCBH(const std::string& name) const;

  template <class THeap>
  size_t TestKPM(const std::string& name) const;

  template <class THeap>
  size_t TestKVM(const std::string& name) const;

 public:
  size_t Test(unsigned type);
  bool TestAll();
};
