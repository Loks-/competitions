#include "tester/tester_graph_ei_distance_positive_cost.h"

#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/hash.h"
#include "common/heap/dheap_ukey_pos_map.h"
#include "common/heap/dheap_ukey_value_map.h"
#include "common/timer.h"

#include "tester/graph_ei_distance_positive_cost.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace {
std::vector<std::string> vnames = {"CBH", "KPM", "DM2", "DM4",
                                   "DM8", "D16", "D32", "D64"};
}  // namespace

TesterGraphEIDistancePositiveCost::TesterGraphEIDistancePositiveCost(
    unsigned graph_size, unsigned edges_per_node)
    : g(graph_size) {
  size_t h = 0, mask = (1u << 30) - 1u;
  for (unsigned i = 0; i < graph_size; ++i) {
    for (unsigned j = 0; j < edges_per_node; ++j) {
      h = hash_combine(h, i);
      g.AddEdge(i, h % graph_size, h & mask);
    }
  }
}

unsigned TesterGraphEIDistancePositiveCost::Size() { return vnames.size(); }

std::string TesterGraphEIDistancePositiveCost::Name(unsigned type) const {
  return vnames[type];
}

size_t TesterGraphEIDistancePositiveCost::TestI(unsigned type) {
  static auto edge_proxy = graph::EdgeCostProxy<uint64_t>();
  size_t h = 0, max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    switch (type) {
      case 0:
        v = DistanceFromSourcePositiveCost_CBH(g, edge_proxy, i, max_cost);
        break;
      case 1:
        v = DistanceFromSourcePositiveCost_KPM<
            heap::DHeapUKeyPosMap<2, uint64_t>>(g, edge_proxy, i, max_cost);
        break;
      case 2:
        v = DistanceFromSourcePositiveCost_KVM<
            heap::DHeapUKeyValueMap<2, uint64_t>>(g, edge_proxy, i, max_cost);
        break;
      case 3:
        v = DistanceFromSourcePositiveCost_KVM<
            heap::DHeapUKeyValueMap<4, uint64_t>>(g, edge_proxy, i, max_cost);
        break;
      case 4:
        v = DistanceFromSourcePositiveCost_KVM<
            heap::DHeapUKeyValueMap<8, uint64_t>>(g, edge_proxy, i, max_cost);
        break;
      case 5:
        v = DistanceFromSourcePositiveCost_KVM<
            heap::DHeapUKeyValueMap<16, uint64_t>>(g, edge_proxy, i, max_cost);
        break;
      case 6:
        v = DistanceFromSourcePositiveCost_KVM<
            heap::DHeapUKeyValueMap<32, uint64_t>>(g, edge_proxy, i, max_cost);
        break;
      case 7:
        v = DistanceFromSourcePositiveCost_KVM<
            heap::DHeapUKeyValueMap<64, uint64_t>>(g, edge_proxy, i, max_cost);
        break;
      default:
        assert(false);
    }
    for (uint64_t d : v) h = hash_combine(h, d);
  }
  return h;
}

size_t TesterGraphEIDistancePositiveCost::Test(unsigned type) {
  Timer t;
  size_t h = TestI(type);
  std::cout << "Test results  [" << Name(type) << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterGraphEIDistancePositiveCost::TestAll() {
  std::unordered_set<size_t> hs;
  for (unsigned i = 0; i < Size(); ++i) hs.insert(Test(i));
  return hs.size() == 1;
}

bool TestGraphEIDistancePositiveCost(bool time_test) {
  if (time_test) {
    TesterGraphEIDistancePositiveCost t1(10000, 4), t2(4000, 1000);
    return t1.TestAll() && t2.TestAll();
  } else {
    TesterGraphEIDistancePositiveCost t(100, 4);
    return t.TestAll();
  }
}
