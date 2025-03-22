#include "tester/tester_graph_ei_distance_unsigned.h"

#include "tester/graph_ei_distance_dijkstra.h"
#include "tester/graph_type.h"

#include "common/graph/graph_ei/distance/dial.h"
#include "common/graph/graph_ei/distance_positive_cost.h"
#include "common/hash/combine.h"
#include "common/heap/base/bucket_queue.h"
#include "common/heap/base/bucket_queue_ll.h"
#include "common/heap/monotone/base/bucket_queue.h"
#include "common/heap/monotone/base/bucket_queue_ll.h"
#include "common/heap/monotone/base/rolling_bucket_queue.h"
#include "common/heap/monotone/base/rolling_bucket_queue_ll.h"
#include "common/heap/monotone/base/two_layers_buckets.h"
#include "common/heap/monotone/base/two_layers_buckets_sqrt.h"
#include "common/heap/monotone/ukvm/bucket_queue.h"
#include "common/heap/monotone/ukvm/bucket_queue_dll.h"
#include "common/heap/monotone/ukvm/radix.h"
#include "common/heap/monotone/ukvm/radix_dll.h"
#include "common/heap/monotone/ukvm/radix_w.h"
#include "common/heap/monotone/ukvm/radix_w_dll.h"
#include "common/heap/monotone/ukvm/rolling_bucket_queue.h"
#include "common/heap/monotone/ukvm/rolling_bucket_queue_dll.h"
#include "common/heap/monotone/ukvm/two_layers_buckets.h"
#include "common/heap/monotone/ukvm/two_layers_buckets_dll.h"
#include "common/heap/monotone/ukvm/two_layers_buckets_sqrt.h"
#include "common/heap/monotone/ukvm/two_layers_buckets_sqrt_dll.h"
#include "common/heap/monotone/ukvm/two_layers_radix_dll.h"
#include "common/heap/monotone/ukvm/two_layers_radix_fibonacci_dll.h"
#include "common/heap/monotone/ukvm/two_layers_radix_mask_dll.h"
#include "common/heap/ukvm/bucket_queue.h"
#include "common/heap/ukvm/bucket_queue_dll.h"
#include "common/timer.h"

#include <iostream>
#include <vector>

TesterGraphEIDistanceUnsigned::TesterGraphEIDistanceUnsigned(
    EGraphType _gtype, unsigned graph_size, unsigned edges_per_node,
    unsigned _max_edge_cost)
    : TesterGraphEIDistanceBasePositive(_gtype, graph_size, edges_per_node,
                                        _max_edge_cost, unsigned(-1u)) {}

void TesterGraphEIDistanceUnsigned::TestDial() {
  Timer t;
  size_t h = 0;
  std::vector<TEdgeCost> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::Dial(g, edge_proxy, i, max_cost, max_edge_cost);
    for (auto d : v) nhash::DCombineH(h, d);
  }
  std::cout << "Test results  [   Dial]: " << h << "\t" << t.get_milliseconds()
            << std::endl;
  hs.insert(h);
}

bool TesterGraphEIDistanceUnsigned::TestAll() {
  PrintGraphType();
  std::cout << "\tMax Edge Cost = " << max_edge_cost << std::endl;
  TestFS(graph::DistanceFromSourcePositiveCost<TGraph, TEdgeCostFunction,
                                               TEdgeCost>,
         "   DPC ");
  TestDial();
  TestDBHP<heap::base::BucketQueue<TEdgeCost>>(" BQ  ");
  TestDBHP<heap::base::BucketQueueLL<TEdgeCost>>(" BQ L");
  TestDBHP<heap::monotone::base::BucketQueue<TEdgeCost>>("MBQ  ");
  TestDBHP<heap::monotone::base::BucketQueueLL<TEdgeCost>>("MBQ L");
  TestDBHPW<heap::monotone::base::RollingBucketQueue<TEdgeCost>>("MRQ  ");
  TestDBHPW<heap::monotone::base::RollingBucketQueueLL<TEdgeCost>>("MRQ L");
  TestDBHP<heap::monotone::base::TwoLayersBuckets<4, TEdgeCost>>("MBQ2 ");
  TestDBHP<heap::monotone::base::TwoLayersBuckets<8, TEdgeCost>>("MBQ3 ");
  TestDBHP<heap::monotone::base::TwoLayersBuckets<16, TEdgeCost>>("MBQ4 ");
  TestDBHP<heap::monotone::base::TwoLayersBuckets<32, TEdgeCost>>("MBQ5 ");
  TestDBHPW<heap::monotone::base::TwoLayersBucketsSqrt<TEdgeCost>>("MBQA ");
  TestDMH<heap::ukvm::BucketQueue>(" BQ  ");
  TestDMH<heap::ukvm::BucketQueueDLL>(" BQ L");
  TestDMH<heap::monotone::ukvm::BucketQueue>("MBQ  ");
  TestDMH<heap::monotone::ukvm::BucketQueueDLL>("MBQ L");
  TestDMHW<heap::monotone::ukvm::RollingBucketQueue>("MRQ  ");
  TestDMHW<heap::monotone::ukvm::RollingBucketQueueDLL>("MRQ L");
  TestDMH<heap::monotone::ukvm::TwoLayersBuckets<4>>("MBQ2 ");
  TestDMH<heap::monotone::ukvm::TwoLayersBuckets<8>>("MBQ3 ");
  TestDMH<heap::monotone::ukvm::TwoLayersBuckets<16>>("MBQ4 ");
  TestDMH<heap::monotone::ukvm::TwoLayersBuckets<32>>("MBQ5 ");
  TestDMHW<heap::monotone::ukvm::TwoLayersBucketsSqrt>("MBQA ");
  TestDMH<heap::monotone::ukvm::TwoLayersBucketsDLL<4>>("MBQ2L");
  TestDMH<heap::monotone::ukvm::TwoLayersBucketsDLL<8>>("MBQ3L");
  TestDMH<heap::monotone::ukvm::TwoLayersBucketsDLL<16>>("MBQ4L");
  TestDMH<heap::monotone::ukvm::TwoLayersBucketsDLL<32>>("MBQ5L");
  TestDMHW<heap::monotone::ukvm::TwoLayersBucketsSqrtDLL>("MBQAL");
  TestDMH<heap::monotone::ukvm::Radix>("MRDX ");
  TestDMH<heap::monotone::ukvm::RadixDLL>("MRDXL");
  TestDMHW<heap::monotone::ukvm::RadixW>("MRDW ");
  TestDMHW<heap::monotone::ukvm::RadixWDLL>("MRDWL");
  TestDMHW<heap::monotone::ukvm::TwoLayersRadixDLL>("MRD2L");
  TestDMHW<heap::monotone::ukvm::TwoLayersRadixMaskDLL>("MRDML");
  TestDMHW<heap::monotone::ukvm::TwoLayersRadixFibonacciDLL>("MRDFL");
  if (gtype == EGraphType::SMALL) {
    TestDijkstraHeaps();
    TestSPFA();
    TestAllPairs();
  }
  return CheckHash();
}

bool TestGraphEIDistanceUnsigned(bool time_test) {
  if (time_test) {
    bool ok = true;
    for (unsigned max_cost : {10, 100, 1000, 10000}) {
      TesterGraphEIDistanceUnsigned t1(EGraphType::SPARSE, 5000, 4, max_cost),
          t2(EGraphType::DENSE, 1000, 500, max_cost),
          t3(EGraphType::LONG_PATH, 1000, 500, max_cost);
      ok = t1.TestAll() && ok;
      ok = t2.TestAll() && ok;
      ok = t3.TestAll() && ok;
    }
    return ok;
  } else {
    TesterGraphEIDistanceUnsigned t(EGraphType::SMALL, 100, 4, 10);
    return t.TestAll();
  }
}
