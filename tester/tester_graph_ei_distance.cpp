#include "tester/tester_graph_ei_distance.h"

#include "tester/graph_type.h"

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
#include "common/graph/graph_ei/distance/spfa/tarjan_pcr.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/replace_edge_info.h"
#include "common/hash.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

TesterGraphEIDistance::TesterGraphEIDistance(EGraphType _gtype,
                                             unsigned graph_size,
                                             unsigned edges_per_node)
    : gtype(_gtype) {
  auto gtemp = CreateHRandomGraph<uint64_t, true>(graph_size, edges_per_node,
                                                  (1u << 30));
  auto vp = nvector::HRandom<uint64_t>(graph_size, 0, (1u << 30));
  auto f = [&](unsigned from, unsigned to, uint64_t cost) {
    return int64_t((vp[from] << 10) - (vp[to] << 10) + cost);
  };
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
  std::cout << "Test results  [BeFo]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestBellmanFordYen() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::BellmanFordYen(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [BFY ]: " << h << "\t" << t.GetMilliseconds()
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
  std::cout << "Test results  [FlWa]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestGoldbergRadzik() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzik(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GR  ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestGoldbergRadzikLazy() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzikLazy(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GRL ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestGoldbergRadzikPCR() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzikPCR(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GRP ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestGoldfarbHaoKai() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldfarbHaoKai(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GHK ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestJohnson() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  auto vv = graph::distance::Johnson(g, edge_proxy, max_cost);
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (int64_t d : vv[i]) h = HashCombine(h, d);
  }
  std::cout << "Test results  [John]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestLevit() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Levit(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Levi]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestPallottino() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Pallottino(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Pall]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestSIPITQ() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SIPITQ(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ SPQ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestSPFA() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SPFA(g, edge_proxy, i, max_cost);
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
    v = graph::distance::spfa::LargeLabelLast(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ LLL]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestSPFASafe() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Safe(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Safe]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestSPFASLF() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SmallLabelFirst(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ SLF]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistance::TestTarjanPCR() const {
  Timer t;
  size_t h = 0;
  int64_t max_cost = (1ll << 60);
  std::vector<int64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::TarjanPCR(g, edge_proxy, i, max_cost);
    for (int64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [TaP ]: " << h << "\t" << t.GetMilliseconds()
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
  hs.insert(TestBellmanFordYen());
  hs.insert(TestSPFA());
  hs.insert(TestSPFASafe());
  hs.insert(TestSIPITQ());
  hs.insert(TestSPFALLL());
  hs.insert(TestSPFASLF());
  hs.insert(TestLevit());
  hs.insert(TestPallottino());
  hs.insert(TestGoldfarbHaoKai());
  hs.insert(TestGoldbergRadzik());
  hs.insert(TestGoldbergRadzikLazy());
  hs.insert(TestGoldbergRadzikPCR());
  hs.insert(TestTarjanPCR());
  if (gtype != EGraphType::SPARSE) hs.insert(TestFloydWarshall());
  hs.insert(TestJohnson());
  return hs.size() == 1;
}

bool TestGraphEIDistance(bool time_test) {
  if (time_test) {
    TesterGraphEIDistance t1(EGraphType::SPARSE, 5000, 4),
        t2(EGraphType::DENSE, 1000, 200);
    return t1.TestAll() && t2.TestAll();
  } else {
    TesterGraphEIDistance t(EGraphType::SMALL, 100, 4);
    return t.TestAll();
  }
}
