#pragma once

#include "tester/graph_type.h"

#include "common/base.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/create_hrandom_graph.h"
#include "common/graph/graph_ei/distance/bellman_ford.h"
#include "common/graph/graph_ei/distance/bellman_ford_yen.h"
#include "common/graph/graph_ei/distance/floyd_warshall.h"
#include "common/graph/graph_ei/distance/johnson.h"
#include "common/graph/graph_ei/distance/spfa/goldberg_radzik.h"
#include "common/graph/graph_ei/distance/spfa/goldberg_radzik_lazy.h"
#include "common/graph/graph_ei/distance/spfa/goldberg_radzik_pcr.h"
#include "common/graph/graph_ei/distance/spfa/goldfarb_hao_kai.h"
#include "common/graph/graph_ei/distance/spfa/large_label_last.h"
#include "common/graph/graph_ei/distance/spfa/levit.h"
#include "common/graph/graph_ei/distance/spfa/pallottino.h"
#include "common/graph/graph_ei/distance/spfa/safe.h"
#include "common/graph/graph_ei/distance/spfa/sipitq.h"
#include "common/graph/graph_ei/distance/spfa/small_label_first.h"
#include "common/graph/graph_ei/distance/spfa/spfa.h"
#include "common/graph/graph_ei/distance/spfa/tarjan.h"
#include "common/graph/graph_ei/distance/spfa/tarjan_pcr.h"
#include "common/graph/graph_ei/distance/spfa/tarjan_pcr_time.h"
#include "common/graph/graph_ei/distance/spfa/tarjan_time.h"
#include "common/graph/graph_ei/distance/spfa/zero_degrees_only.h"
#include "common/graph/graph_ei/distance/spfa/zero_degrees_only_base.h"
#include "common/graph/graph_ei/distance/spfa/zero_degrees_only_time.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/hash.h"
#include "common/timer.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

template <class TTEdgeCost, bool _directed_edges>
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
  std::unordered_set<size_t> hs;

 public:
  TesterGraphEIDistanceBase(EGraphType _gtype, unsigned graph_size,
                            unsigned edges_per_node, TEdgeCost _max_edge_cost,
                            TEdgeCost _max_cost)
      : max_edge_cost(_max_edge_cost),
        max_cost(_max_cost),
        gtype(_gtype),
        g(CreateHRandomGraph<TTEdgeCost, _directed_edges>(
            graph_size, edges_per_node, max_edge_cost)) {}

 protected:
  bool CheckHash() const { return hs.size() == 1; }

  template <class TFunction>
  void TestFA(TFunction& fa, const std::string& name) {
    Timer t;
    size_t h = 0;
    auto vv = fa(g, edge_proxy, max_cost);
    for (unsigned i = 0; i < g.Size(); ++i) {
      for (auto d : vv[i]) h = HashCombine(h, d);
    }
    std::cout << "Test results  [" << name << "]: " << h << "\t"
              << t.GetMilliseconds() << std::endl;
    hs.insert(h);
  }

  template <class TFunction>
  void TestFS(TFunction& fs, const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < g.Size(); ++i) {
      v = fs(g, edge_proxy, i, max_cost);
      for (auto d : v) h = HashCombine(h, d);
    }
    std::cout << "Test results  [" << name << "]: " << h << "\t"
              << t.GetMilliseconds() << std::endl;
    hs.insert(h);
  }

 protected:
  void TestAllPairs() {
    if (gtype != EGraphType::SPARSE)
      TestFA(
          graph::distance::FloydWarshall<TGraph, TEdgeCostFunction, TEdgeCost>,
          "A  FlWa");
    TestFA(graph::distance::Johnson<TGraph, TEdgeCostFunction, TEdgeCost>,
           "A  John");
  }

  void TestSPFA() {
    TestFS(graph::distance::BellmanFord<TGraph, TEdgeCostFunction, TEdgeCost>,
           "S  BF  ");
    TestFS(
        graph::distance::BellmanFordYen<TGraph, TEdgeCostFunction, TEdgeCost>,
        "S  BFY ");
    TestFS(graph::distance::spfa::SPFA<TGraph, TEdgeCostFunction, TEdgeCost>,
           "S  SPFA");
    TestFS(graph::distance::spfa::Safe<TGraph, TEdgeCostFunction, TEdgeCost>,
           "S  Safe");
    TestFS(graph::distance::spfa::SIPITQ<TGraph, TEdgeCostFunction, TEdgeCost>,
           "S   SPQ");
    TestFS(graph::distance::spfa::LargeLabelLast<TGraph, TEdgeCostFunction,
                                                 TEdgeCost>,
           "S   LLL");
    TestFS(graph::distance::spfa::SmallLabelFirst<TGraph, TEdgeCostFunction,
                                                  TEdgeCost>,
           "S   SLF");
    TestFS(graph::distance::spfa::Levit<TGraph, TEdgeCostFunction, TEdgeCost>,
           "S  Levi");
    TestFS(
        graph::distance::spfa::Pallottino<TGraph, TEdgeCostFunction, TEdgeCost>,
        "S  Pall");
    TestFS(graph::distance::spfa::GoldfarbHaoKai<TGraph, TEdgeCostFunction,
                                                 TEdgeCost>,
           "S  GHK ");
    TestFS(graph::distance::spfa::GoldbergRadzik<TGraph, TEdgeCostFunction,
                                                 TEdgeCost>,
           "S  GR  ");
    TestFS(graph::distance::spfa::GoldbergRadzikLazy<TGraph, TEdgeCostFunction,
                                                     TEdgeCost>,
           "S  GRL ");
    TestFS(graph::distance::spfa::GoldbergRadzikPCR<TGraph, TEdgeCostFunction,
                                                    TEdgeCost>,
           "S  GRP ");
    TestFS(graph::distance::spfa::Tarjan<TGraph, TEdgeCostFunction, TEdgeCost>,
           "S  Ta  ");
    TestFS(
        graph::distance::spfa::TarjanTime<TGraph, TEdgeCostFunction, TEdgeCost>,
        "S  Ta T");
    TestFS(
        graph::distance::spfa::TarjanPCR<TGraph, TEdgeCostFunction, TEdgeCost>,
        "S  TaP ");
    TestFS(graph::distance::spfa::TarjanPCRTime<TGraph, TEdgeCostFunction,
                                                TEdgeCost>,
           "S  TaPT");
    TestFS(graph::distance::spfa::ZeroDegreesOnlyBase<TGraph, TEdgeCostFunction,
                                                      TEdgeCost>,
           "S  ZDOB");
    TestFS(graph::distance::spfa::ZeroDegreesOnly<TGraph, TEdgeCostFunction,
                                                  TEdgeCost>,
           "S  ZDO ");
    TestFS(graph::distance::spfa::ZeroDegreesOnlyTime<TGraph, TEdgeCostFunction,
                                                      TEdgeCost>,
           "S  ZDOT");
  }
};
