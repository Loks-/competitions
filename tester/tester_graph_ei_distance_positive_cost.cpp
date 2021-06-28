#include "tester/tester_graph_ei_distance_positive_cost.h"

#include "tester/graph_type.h"

#include "common/graph/graph_ei/distance_positive_cost.h"

#include <iostream>

TesterGraphEIDistancePositiveCost::TesterGraphEIDistancePositiveCost(
    EGraphType _gtype, unsigned graph_size, unsigned edges_per_node)
    : TesterGraphEIDistanceBasePositive(_gtype, graph_size, edges_per_node,
                                        (1u << 30), uint64_t(-1ull)) {}

bool TesterGraphEIDistancePositiveCost::TestAll() {
  switch (gtype) {
    case EGraphType::SMALL:
      std::cout << "Small:" << std::endl;
      break;
    case EGraphType::SPARSE:
      std::cout << "Sparse:" << std::endl;
      break;
    case EGraphType::DENSE:
      std::cout << "Dense:" << std::endl;
      break;
    default:
      assert(false);
  }
  TestFS(graph::DistanceFromSourcePositiveCost<TGraph, TEdgeCostFunction,
                                               TEdgeCost>,
         "   DPC ");
  TestDijkstraHeaps();
  TestSPFA();
  TestAllPairs();
  return CheckHash();
}

bool TestGraphEIDistancePositiveCost(bool time_test) {
  if (time_test) {
    TesterGraphEIDistancePositiveCost t1(EGraphType::SPARSE, 5000, 4),
        t2(EGraphType::DENSE, 1000, 400);
    return t1.TestAll() && t2.TestAll();
  } else {
    TesterGraphEIDistancePositiveCost t(EGraphType::SMALL, 100, 4);
    return t.TestAll();
  }
}
