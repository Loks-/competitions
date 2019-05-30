#pragma once

#include "common/base.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include "tester/graph_type.h"

#include <string>
#include <vector>

class TesterMinimumSpanningTree {
 public:
  using TGraph = graph::GraphEI<uint64_t, false>;
  using TEdgeCostFunction = graph::EdgeCostProxy<uint64_t>;

 protected:
  EGraphType gtype;
  TGraph g;
  TEdgeCostFunction edge_proxy;

 public:
  TesterMinimumSpanningTree(EGraphType _gtype, unsigned graph_size,
                            unsigned edges_per_node);

 protected:
  uint64_t TestKruskal() const;
  uint64_t TestPrimBaseBinaryHeap() const;

  template <template <class> class THeap>
  uint64_t TestPrimDHeap(const std::string& name) const;

  template <class THeap>
  uint64_t TestPrimKPM(const std::string& name) const;

 public:
  bool TestAll();
};
