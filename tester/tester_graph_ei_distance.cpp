#include "tester/tester_graph_ei_distance.h"

#include "tester/graph_type.h"

#include "common/graph/graph_ei/create_hrandom_graph.h"
#include "common/graph/graph_ei/distance/bellman_ford.h"
#include "common/graph/graph_ei/distance/floyd_warshall.h"
#include "common/graph/graph_ei/distance/spfa.h"
#include "common/graph/graph_ei/distance/spfa_lll.h"
#include "common/graph/graph_ei/distance/spfa_slf.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/replace_edge_info.h"
#include "common/vector/hrandom.h"
#include "common/hash.h"
#include "common/timer.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


TesterGraphEIDistance::TesterGraphEIDistance(
    EGraphType _gtype, unsigned graph_size, unsigned edges_per_node)
    : gtype(_gtype) {
    auto gtemp = CreateHRandomGraph<uint64_t, true>(graph_size, edges_per_node,
                                           (1u << 30));
    auto vp = nvector::HRandom<uint64_t>(graph_size, 0, (1u << 30));
    auto f = [&](unsigned from, unsigned to, uint64_t cost) { 
        return int64_t((vp[from] << 10) - (vp[to] << 10) + cost); };
    g = ReplaceEdgeInfo<uint64_t, int64_t>(gtemp, f);
}

size_t TesterGraphEIDistance::TestBellmanFord() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::BellmanFord(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [BlFd]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestFloydWarshall() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  auto vv = graph::distance::FloydWarshall(g, edge_proxy, max_cost);
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (int64_t d : vv[i]) h = HashCombine(h, d);
  }
  std::cout << "Test results  [FLYD]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestSPFA() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::SPFA(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [SPFA]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestSPFALLL() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::SPFALLL(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ LLL]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestSPFASLF() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::SPFASLF(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ SLF]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

bool TesterGraphEIDistance::TestAll() {
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
  std::unordered_set<size_t> hs;
  hs.insert(TestBellmanFord());
  hs.insert(TestSPFA());
  hs.insert(TestSPFALLL());
  hs.insert(TestSPFASLF());
  if (gtype != EGraphType::SPARSE) hs.insert(TestFloydWarshall());
  return hs.size() == 1;
}

bool TestGraphEIDistance(bool time_test) {
  if (time_test) {
    TesterGraphEIDistance t1(EGraphType::SPARSE, 2000, 4),
        t2(EGraphType::DENSE, 400, 100);
    return t1.TestAll() && t2.TestAll();
  } else {
    TesterGraphEIDistance t(EGraphType::SMALL, 100, 4);
    return t.TestAll();
  }
}
