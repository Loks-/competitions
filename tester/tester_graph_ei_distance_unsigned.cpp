#include "tester/tester_graph_ei_distance_unsigned.h"

#include "tester/graph_ei_distance_positive_cost.h"
#include "tester/graph_type.h"

#include "common/graph/graph_ei/create_hrandom_graph.h"
#include "common/graph/graph_ei/distance/bellman_ford.h"
#include "common/graph/graph_ei/distance/bellman_ford_yen.h"
#include "common/graph/graph_ei/distance/dial.h"
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
#include "common/heap/base/bucket_queue.h"
#include "common/heap/base/dheap.h"
#include "common/heap/ext/dheap_ukey_pos_map.h"
#include "common/heap/monotone/base/bucket_queue.h"
#include "common/heap/monotone/ukvm/bucket_queue.h"
#include "common/heap/ukvm/binomial.h"
#include "common/heap/ukvm/bucket_queue.h"
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
    heap::ukvm::Pairing<unsigned, std::less<unsigned>, multipass, auxiliary>;

TesterGraphEIDistanceUnsigned::TesterGraphEIDistanceUnsigned(
    EGraphType _gtype, unsigned graph_size, unsigned edges_per_node,
    unsigned _max_edge_cost)
    : max_edge_cost(_max_edge_cost),
      gtype(_gtype),
      g(CreateHRandomGraph<unsigned, true>(graph_size, edges_per_node,
                                           max_edge_cost)) {}

size_t TesterGraphEIDistanceUnsigned::TestBellmanFord() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::BellmanFord(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [BeFo]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestBellmanFordYen() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::BellmanFordYen(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [BFY ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestDial() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::Dial(g, edge_proxy, i, max_cost, max_edge_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Dial]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestFloydWarshall() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  auto vv = graph::distance::FloydWarshall(g, edge_proxy, max_cost);
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (uint64_t d : vv[i]) h = HashCombine(h, d);
  }
  std::cout << "Test results  [FlWa]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestGoldbergRadzik() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzik(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GR  ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestGoldbergRadzikLazy() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzikLazy(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GRL ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestGoldbergRadzikPCR() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldbergRadzikPCR(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GRP ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestGoldfarbHaoKai() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::GoldfarbHaoKai(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [GHK ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestJohnson() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  auto vv = graph::distance::Johnson(g, edge_proxy, max_cost);
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (uint64_t d : vv[i]) h = HashCombine(h, d);
  }
  std::cout << "Test results  [John]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestLevit() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Levit(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Levi]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestPallottino() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Pallottino(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Pall]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestSIPITQ() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SIPITQ(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ SPQ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestSPFA() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SPFA(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [SPFA]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestSPFALLL() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::LargeLabelLast(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ LLL]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestSPFASafe() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Safe(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Safe]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestSPFASLF() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::SmallLabelFirst(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ SLF]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestTarjan() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::Tarjan(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Ta  ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestTarjanPCR() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::TarjanPCR(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [TaP ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestTarjanPCRTime() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::TarjanPCRTime(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [TaPT]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestTarjanTime() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::TarjanTime(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [Ta T]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestZDO() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::ZeroDegreesOnly(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ZDO ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestZDOBase() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::ZeroDegreesOnlyBase(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ZDOB]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterGraphEIDistanceUnsigned::TestZDOTime() const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = graph::distance::spfa::ZeroDegreesOnlyTime(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [ZDOT]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

template <template <class TData> class THeap>
size_t TesterGraphEIDistanceUnsigned::TestHP(const std::string& name) const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_HP<THeap>(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistanceUnsigned::TestHPV(const std::string& name) const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_HPV<THeap>(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistanceUnsigned::TestMHPV(const std::string& name) const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_MHPV<THeap>(g, edge_proxy, i,
                                                   max_edge_cost, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistanceUnsigned::TestKPM(const std::string& name) const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_KPM<THeap>(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistanceUnsigned::TestKVM(const std::string& name) const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_KVM<THeap>(g, edge_proxy, i, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterGraphEIDistanceUnsigned::TestMKVM(const std::string& name) const {
  Timer t;
  size_t h = 0;
  unsigned max_cost = -1u;
  std::vector<unsigned> v;
  for (unsigned i = 0; i < g.Size(); ++i) {
    v = DistanceFromSourcePositiveCost_MKVM<THeap>(g, edge_proxy, i,
                                                   max_edge_cost, max_cost);
    for (unsigned d : v) h = HashCombine(h, d);
  }
  std::cout << "Test results  [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterGraphEIDistanceUnsigned::TestAll() {
  switch (gtype) {
    case EGraphType::SMALL:
      std::cout << "Small " << max_edge_cost << ":" << std::endl;
      break;
    case EGraphType::SPARSE:
      std::cout << "Sparse " << max_edge_cost << ":" << std::endl;
      break;
    case EGraphType::DENSE:
      std::cout << "Dense " << max_edge_cost << ":" << std::endl;
      break;
    default:
      assert(false);
  }
  std::unordered_set<size_t> hs;
  hs.insert(TestHP<TBinaryHeap>("  BH"));
  hs.insert(TestHP<TDHeap2>("HP 2"));
  hs.insert(TestHP<TDHeap4>("HP 4"));
  hs.insert(TestHP<TDHeap8>("HP 8"));
  hs.insert(TestHP<TDHeap16>("HP16"));
  hs.insert(TestHPV<heap::base::BucketQueue<unsigned>>(" BQ "));
  hs.insert(TestMHPV<heap::monotone::base::BucketQueue<unsigned>>("MBQ "));
  hs.insert(TestKPM<heap::ext::DHeapUKeyPosMap<2, unsigned>>("KPM2"));
  hs.insert(TestKPM<heap::ext::DHeapUKeyPosMap<4, unsigned>>("KPM4"));
  hs.insert(TestKPM<heap::ext::DHeapUKeyPosMap<8, unsigned>>("KPM8"));
  hs.insert(TestKVM<heap::ukvm::DHeap<2, unsigned>>("DM 2"));
  hs.insert(TestKVM<heap::ukvm::DHeap<4, unsigned>>("DM 4"));
  hs.insert(TestKVM<heap::ukvm::DHeap<8, unsigned>>("DM 8"));
  hs.insert(TestKVM<heap::ukvm::DHeap<16, unsigned>>("DM16"));
  hs.insert(TestKVM<heap::ukvm::BucketQueue>(" BM "));
  hs.insert(TestMKVM<heap::monotone::ukvm::BucketQueue>("MBM "));
  hs.insert(TestKVM<heap::ukvm::Binomial<unsigned>>("BNML"));
  hs.insert(TestKVM<heap::ukvm::Fibonacci<unsigned>>("FBNC"));
  hs.insert(TestKVM<TPairing<0, 0>>("PR00"));
  hs.insert(TestKVM<TPairing<1, 0>>("PR01"));
  hs.insert(TestKVM<TPairing<0, 1>>("PR10"));
  hs.insert(TestKVM<TPairing<1, 1>>("PR11"));
  hs.insert(TestDial());
  if (gtype != EGraphType::SPARSE)
    hs.insert(TestKVM<heap::ukvm::UnorderedSet<unsigned>>("USet"));
  if (gtype == EGraphType::SMALL) hs.insert(TestBellmanFord());
  if (gtype == EGraphType::SMALL) hs.insert(TestBellmanFordYen());
  hs.insert(TestSPFA());
  if (gtype == EGraphType::SMALL) hs.insert(TestSPFASafe());
  if (gtype == EGraphType::SMALL) hs.insert(TestSIPITQ());
  if (gtype == EGraphType::SMALL) hs.insert(TestSPFALLL());
  if (gtype == EGraphType::SMALL) hs.insert(TestSPFASLF());
  if (gtype == EGraphType::SMALL) hs.insert(TestLevit());
  if (gtype == EGraphType::SMALL) hs.insert(TestPallottino());
  if (gtype == EGraphType::SMALL) hs.insert(TestGoldfarbHaoKai());
  if (gtype == EGraphType::SMALL) hs.insert(TestGoldbergRadzik());
  if (gtype == EGraphType::SMALL) hs.insert(TestGoldbergRadzikLazy());
  if (gtype == EGraphType::SMALL) hs.insert(TestGoldbergRadzikPCR());
  if (gtype == EGraphType::SMALL) hs.insert(TestTarjan());
  if (gtype == EGraphType::SMALL) hs.insert(TestTarjanTime());
  if (gtype == EGraphType::SMALL) hs.insert(TestTarjanPCR());
  if (gtype == EGraphType::SMALL) hs.insert(TestTarjanPCRTime());
  if (gtype == EGraphType::SMALL) hs.insert(TestZDOBase());
  if (gtype == EGraphType::SMALL) hs.insert(TestZDO());
  if (gtype == EGraphType::SMALL) hs.insert(TestZDOTime());
  if (gtype != EGraphType::SPARSE) hs.insert(TestFloydWarshall());
  hs.insert(TestJohnson());
  return hs.size() == 1;
}

bool TestGraphEIDistanceUnsigned(bool time_test) {
  if (time_test) {
    bool ok = true;
    for (unsigned max_cost : {10, 100, 1000}) {
      TesterGraphEIDistanceUnsigned t1(EGraphType::SPARSE, 5000, 4, max_cost),
          t2(EGraphType::DENSE, 1000, 400, max_cost);
      ok = t1.TestAll() && ok;
      ok = t2.TestAll() && ok;
    }
    return ok;
  } else {
    TesterGraphEIDistanceUnsigned t(EGraphType::SMALL, 100, 4, 10);
    return t.TestAll();
  }
}
