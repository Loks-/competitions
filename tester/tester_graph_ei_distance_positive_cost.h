#pragma once

#include "tester/graph_type.h"

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
  EGraphType gtype;
  TGraph g;
  TEdgeCostFunction edge_proxy;

 public:
  TesterGraphEIDistancePositiveCost(EGraphType _gtype, unsigned graph_size,
                                    unsigned edges_per_node);

 protected:
  size_t TestBellmanFord() const;
  size_t TestBellmanFordYen() const;
  size_t TestFloydWarshall() const;
  size_t TestGoldbergRadzik() const;
  size_t TestGoldbergRadzikLazy() const;
  size_t TestGoldbergRadzikPCR() const;
  size_t TestGoldfarbHaoKai() const;
  size_t TestJohnson() const;
  size_t TestLevit() const;
  size_t TestPallottino() const;
  size_t TestSIPITQ() const;
  size_t TestSPFA() const;
  size_t TestSPFALLL() const;
  size_t TestSPFASLF() const;
  size_t TestTarjan() const;

  template <template <class TData> class THeap>
  size_t TestCBH(const std::string& name) const;

  template <class THeap>
  size_t TestKPM(const std::string& name) const;

  template <class THeap>
  size_t TestKVM(const std::string& name) const;

 public:
  bool TestAll();
};
