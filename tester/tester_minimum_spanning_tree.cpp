#include "tester/tester_minimum_spanning_tree.h"

#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/mst/kruskal.h"
#include "common/hash.h"
#include "common/timer.h"

#include "tester/minimum_spanning_tree.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace {
std::vector<std::string> vnames = {"Kruskal", "Prim_BH", "Prim_CBH"};
}  // namespace

TesterMinimumSpanningTree::TesterMinimumSpanningTree(unsigned graph_size,
                                                     unsigned edges_per_node)
    : g(graph_size) {
  size_t h = 0, mask = (1u << 30) - 1u;
  for (unsigned i = 0; i < graph_size; ++i) {
    for (unsigned j = 0; j < (edges_per_node + (i % 2)) / 2; ++j) {
      h = hash_combine(h, i);
      g.AddEdge(i, h % graph_size, h & mask);
    }
  }
}

unsigned TesterMinimumSpanningTree::Size() { return vnames.size(); }

std::string TesterMinimumSpanningTree::Name(unsigned type) const {
  return vnames[type];
}

uint64_t TesterMinimumSpanningTree::TestI(unsigned type) {
  static auto edge_proxy = graph::EdgeCostProxy<uint64_t>();
  switch (type) {
    case 0:
      return graph::mst::Kruskal(g, edge_proxy).second;
    case 1:
      return MinimumSpanningTree_Prim_BH(g, edge_proxy).second;
    case 2:
      return MinimumSpanningTree_Prim_CBH(g, edge_proxy, -1ull).second;
    default:
      assert(false);
      return 0;
  }
}

uint64_t TesterMinimumSpanningTree::Test(unsigned type) {
  Timer t;
  uint64_t r = TestI(type);
  std::string name = Name(type);
  name.resize(10, ' ');
  std::cout << "Test results " << name << ": " << r << "\t"
            << t.GetMilliseconds() << std::endl;
  return r;
}

bool TesterMinimumSpanningTree::TestAll() {
  std::unordered_set<uint64_t> hs;
  for (unsigned i = 0; i < Size(); ++i) hs.insert(Test(i));
  return hs.size() == 1;
}

bool TestMinimumSpanningTree(bool time_test) {
  if (time_test) {
    TesterMinimumSpanningTree t1(1000000, 10), t2(4000, 2000);
    return t1.TestAll() && t2.TestAll();
  } else {
    TesterMinimumSpanningTree t(100, 10);
    return t.TestAll();
  }
}
