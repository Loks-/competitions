#include "tester/tester_graph_ei_distance_positive_cost.h"

#include "tester/graph_type.h"

#include "common/graph/graph_ei/distance_positive_cost.h"
#include "common/heap/monotone/ukvm/radix_w_dll.h"
#include "common/heap/monotone/ukvm/two_layers_radix_fibonacci_dll.h"
#include "common/heap/monotone/ukvm/two_layers_radix_mask_dll.h"

#include <iostream>

TesterGraphEIDistancePositiveCost::TesterGraphEIDistancePositiveCost(
    EGraphType _gtype, unsigned graph_size, unsigned edges_per_node)
    : TesterGraphEIDistanceBasePositive(_gtype, graph_size, edges_per_node,
                                        (1u << 20), unsigned(-1u)) {}

bool TesterGraphEIDistancePositiveCost::TestAll() {
  PrintGraphType();
  TestFS(graph::DistanceFromSourcePositiveCost<TGraph, TEdgeCostFunction,
                                               TEdgeCost>,
         "   DPC ");
  TestDijkstraHeaps();
  TestDMHW<heap::monotone::ukvm::RadixWDLL>("MRDWL");
  TestDMHW<heap::monotone::ukvm::TwoLayersRadixMaskDLL>("MRDML");
  TestDMHW<heap::monotone::ukvm::TwoLayersRadixFibonacciDLL>("MRDFL");
  if (gtype != EGraphType::LONG_PATH) TestSPFA();
  TestAllPairs();
  return CheckHash();
}

bool TestGraphEIDistancePositiveCost(bool time_test) {
  if (time_test) {
    TesterGraphEIDistancePositiveCost t1(EGraphType::SPARSE, 5000, 4),
        t2(EGraphType::DENSE, 1000, 400), t3(EGraphType::LONG_PATH, 1000, 500);
    return t1.TestAll() && t2.TestAll() && t3.TestAll();
  } else {
    TesterGraphEIDistancePositiveCost t(EGraphType::SMALL, 100, 4);
    return t.TestAll();
  }
}
