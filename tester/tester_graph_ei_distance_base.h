#pragma once

#include "tester/graph_type.h"

#include "common/base.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/create_hrandom_graph.h"
#include "common/timer.h"

#include <string>
#include <vector>

template<class TTEdgeCost, bool _directed_edges>
class TesterGraphEIDistanceBase {
 public:
  static const bool directed_edges = _directed_edges;
  using TEdgeCost = TTEdgeCost;
  using TGraph = graph::GraphEI<TEdgeCost, directed_edges>;
  using TEdgeCostFunction = graph::EdgeCostProxy<TEdgeCost>;

 protected:
  TEdgeCost max_edge_cost, max_cost;
  EGraphType gtype;
  TGraph g;
  TEdgeCostFunction edge_proxy;

 public:
  TesterGraphEIDistanceBase(EGraphType _gtype, unsigned graph_size,
                                unsigned edges_per_node,
                                TEdgeCost _max_edge_cost,
                                TEdgeCost _max_cost) :
      max_edge_cost(_max_edge_cost),
      max_cost(_max_cost),
      gtype(_gtype),
      g(CreateHRandomGraph<TTEdgeCost, _directed_edges>(graph_size, 
                                           edges_per_node,
                                           max_edge_cost)) {}

 protected:
  template<typename TFunction>
  size_t TestFS(TFunction& fs, const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < g.Size(); ++i) {
        v = fs(g, edge_proxy, i, max_cost);
        for (TEdgeCost d : v) h = HashCombine(h, d);
    }
    std::cout << "Test results  [" << name << "]: " << h << "\t" << t.GetMilliseconds()
                << std::endl;
    return h;
  }

 protected:
  size_t TestBellmanFord() const;
  size_t TestBellmanFordYen() const;
  size_t TestDial() const;
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
  size_t TestDBHP(const std::string& name) const;

  template <class THeap>
  size_t TestDBHPW(const std::string& name) const;

  template <class THeap>
  size_t TestDEH(const std::string& name) const;

  template <class THeap>
  size_t TestDMH(const std::string& name) const;

  template <class THeap>
  size_t TestDMHW(const std::string& name) const;

 public:
  bool TestAll();
};
