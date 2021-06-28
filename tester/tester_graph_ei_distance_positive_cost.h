#pragma once

#include "tester/graph_type.h"
#include "tester/tester_graph_ei_distance_base.h"

#include "common/base.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <string>
#include <vector>

class TesterGraphEIDistancePositiveCost : public TesterGraphEIDistanceBase<uint64_t, true> {
 public:
  using TBase = TesterGraphEIDistanceBase<unsigned, true>;
  using TGraph = typename TBase::TGraph;
  using TEdgeCostFunction = typename TBase::TEdgeCostFunction;

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
  size_t TestSPFASafe() const;
  size_t TestSPFASLF() const;
  size_t TestTarjan() const;
  size_t TestTarjanPCR() const;
  size_t TestTarjanPCRTime() const;
  size_t TestTarjanTime() const;
  size_t TestZDO() const;
  size_t TestZDOBase() const;
  size_t TestZDOTime() const;

  template <template <class TData> class THeap>
  size_t TestDBH(const std::string& name) const;

  template <class THeap>
  size_t TestDEH(const std::string& name) const;

  template <class THeap>
  size_t TestDMH(const std::string& name) const;

 public:
  bool TestAll();
};
