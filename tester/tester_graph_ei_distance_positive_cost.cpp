#include "tester/tester_graph_ei_distance_positive_cost.h"

#include "tester/graph_ei_distance_positive_cost.h"
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
#include "common/graph/graph_ei/distance/spfa/tarjan.h"
#include "common/graph/graph_ei/distance/spfa/tarjan_pcr.h"
#include "common/graph/graph_ei/distance/spfa/tarjan_pcr_time.h"
#include "common/graph/graph_ei/distance/spfa/tarjan_time.h"
#include "common/graph/graph_ei/distance/spfa/zero_degrees_only.h"
#include "common/graph/graph_ei/distance/spfa/zero_degrees_only_base.h"
#include "common/graph/graph_ei/distance/spfa/zero_degrees_only_time.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/hash.h"
#include "common/heap/base/binary.h"
#include "common/heap/base/dheap.h"
#include "common/heap/ext/dheap_ukey_pos_map.h"
#include "common/heap/ukvm/binomial.h"
#include "common/heap/ukvm/complete_binary_tree.h"
#include "common/heap/ukvm/dheap.h"
#include "common/heap/ukvm/fibonacci.h"
#include "common/heap/ukvm/pairing.h"
#include "common/heap/ukvm/unordered_set.h"
#include "common/timer.h"

#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

template <class TData>
using TBinaryHeap = heap::base::Binary<TData>;
template <class TData>
using TDHeap2 = heap::base::DHeap<2u, TData>;
template <class TData>
using TDHeap4 = heap::base::DHeap<4u, TData>;
template <class TData>
using TDHeap8 = heap::base::DHeap<8u, TData>;
template <class TData>
using TDHeap16 = heap::base::DHeap<16u, TData>;
template <bool multipass, bool auxiliary>
using TPairing =
    heap::ukvm::Pairing<uint64_t, std::less<uint64_t>, multipass, auxiliary>;

TesterGraphEIDistancePositiveCost::TesterGraphEIDistancePositiveCost(
    EGraphType _gtype, unsigned graph_size, unsigned edges_per_node)
    :   TesterGraphEIDistanceBase(_gtype, graph_size, edges_per_node, (1u << 30), uint64_t(-1ull)) {}

size_t TesterGraphEIDistancePositiveCost::TestBellmanFord() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::BellmanFord(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [   BF  ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestBellmanFordYen() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::BellmanFordYen(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [   BFY ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestFloydWarshall() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  auto vv = graph::distance::FloydWarshall(g, edge_proxy, max_cost);
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (uint64_t d : vv[i]) h = HashCombine(h, d);
  }
  std::cout << "Test results  [A  FlWa]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestGoldbergRadzik() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzik(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  GR  ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestGoldbergRadzikLazy() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzikLazy(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  GRL ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestGoldbergRadzikPCR() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzikPCR(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  GRP ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestGoldfarbHaoKai() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldfarbHaoKai(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  GHK ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestJohnson() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  auto vv = graph::distance::Johnson(g, edge_proxy, max_cost);
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (uint64_t d : vv[i]) h = HashCombine(h, d);
  }
  std::cout << "Test results  [A  John]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestLevit() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Levit(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  Levi]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestPallottino() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Pallottino(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  Pall]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestSIPITQ() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SIPITQ(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S   SPQ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestSPFA() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SPFA(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  SPFA]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestSPFALLL() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::LargeLabelLast(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S   LLL]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestSPFASafe() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Safe(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  Safe]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestSPFASLF() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SmallLabelFirst(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S   SLF]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestTarjan() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Tarjan(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  Ta  ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestTarjanPCR() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::TarjanPCR(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  TaP ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestTarjanPCRTime() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::TarjanPCRTime(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  TaPT]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestTarjanTime() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::TarjanTime(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  Ta T]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestZDO() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::ZeroDegreesOnly(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  ZDO ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestZDOBase() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::ZeroDegreesOnlyBase(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  ZDOB]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistancePositiveCost::TestZDOTime() const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::ZeroDegreesOnlyTime(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [S  ZDOT]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

template <template <class TData> class THeap>
size_t TesterGraphEIDistancePositiveCost::TestDBH(
    const std::string& name) const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_DBH<THeap>(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [DB" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistancePositiveCost::TestDEH(
    const std::string& name) const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_DEH<THeap>(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [DE" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistancePositiveCost::TestDMH(
    const std::string& name) const {
  Timer t;
  size_t h = 0;
  uint64_t max_cost = -1ull;
  std::vector<uint64_t> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_DMH<THeap>(g, edge_proxy, i, max_cost);
    for (uint64_t d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [DM" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

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
  std::unordered_set<size_t> hs;
  hs.insert(TestDBH<TBinaryHeap>("  BH "));
  hs.insert(TestDBH<TDHeap2>(" DH1 "));
  hs.insert(TestDBH<TDHeap4>(" DH2 "));
  hs.insert(TestDBH<TDHeap8>(" DH3 "));
  hs.insert(TestDBH<TDHeap16>(" DH4 "));
  hs.insert(TestDEH<heap::ext::DHeapUKeyPosMap<2, uint64_t>>(" DH1 "));
  hs.insert(TestDEH<heap::ext::DHeapUKeyPosMap<4, uint64_t>>(" DH2 "));
  hs.insert(TestDEH<heap::ext::DHeapUKeyPosMap<8, uint64_t>>(" DH3 "));
  hs.insert(TestDEH<heap::ext::DHeapUKeyPosMap<16, uint64_t>>(" DH4 "));
  hs.insert(TestDMH<heap::ukvm::DHeap<2, uint64_t>>(" DH1 "));
  hs.insert(TestDMH<heap::ukvm::DHeap<4, uint64_t>>(" DH2 "));
  hs.insert(TestDMH<heap::ukvm::DHeap<8, uint64_t>>(" DH3 "));
  hs.insert(TestDMH<heap::ukvm::DHeap<16, uint64_t>>(" DH4 "));
  hs.insert(TestDMH<heap::ukvm::CompleteBinaryTree<uint64_t>>(" CBT "));
  hs.insert(TestDMH<heap::ukvm::Binomial<uint64_t>>(" BNML"));
  hs.insert(TestDMH<heap::ukvm::Fibonacci<uint64_t>>(" FBNC"));
  hs.insert(TestDMH<TPairing<0, 0>>(" PR00"));
  hs.insert(TestDMH<TPairing<1, 0>>(" PR01"));
  hs.insert(TestDMH<TPairing<0, 1>>(" PR10"));
  hs.insert(TestDMH<TPairing<1, 1>>(" PR11"));
  if (gtype != EGraphType::SPARSE)
    hs.insert(TestDMH<heap::ukvm::UnorderedSet<uint64_t>>(" USet"));
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
  hs.insert(TestTarjan());
  hs.insert(TestTarjanTime());
  hs.insert(TestTarjanPCR());
  hs.insert(TestTarjanPCRTime());
  hs.insert(TestZDOBase());
  hs.insert(TestZDO());
  hs.insert(TestZDOTime());
  if (gtype != EGraphType::SPARSE) hs.insert(TestFloydWarshall());
  hs.insert(TestJohnson());
  return hs.size() == 1;
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
