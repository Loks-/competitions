#pragma once

#include "tester/graph_ei_distance_dijkstra.h"
#include "tester/graph_type.h"
#include "tester/tester_graph_ei_distance_base.h"

#include "common/base.h"
#include "common/hash/combine.h"
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

template <class TEdgeCost, bool _directed_edges>
class TesterGraphEIDistanceBasePositive
    : public TesterGraphEIDistanceBase<TEdgeCost, _directed_edges> {
 public:
  using TBase = TesterGraphEIDistanceBase<TEdgeCost, _directed_edges>;

 protected:
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
  using TPairing = heap::ukvm::Pairing<TEdgeCost, std::less<TEdgeCost>,
                                       multipass, auxiliary>;

 public:
  TesterGraphEIDistanceBasePositive(EGraphType _gtype, unsigned graph_size,
                                    unsigned edges_per_node,
                                    TEdgeCost _max_edge_cost,
                                    TEdgeCost _max_cost)
      : TBase(_gtype, graph_size, edges_per_node, _max_edge_cost, _max_cost) {}

 protected:
  template <template <class TData> class THeap>
  void TestDBH(const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < TBase::g.Size(); ++i) {
      v = Dijkstra_DBH<THeap>(TBase::g, TBase::edge_proxy, i, TBase::max_cost);
      for (auto d : v) nhash::DCombineH(h, d);
    }
    std::cout << "Test results  [DB" << name << "]: " << h << "\t"
              << t.get_milliseconds() << std::endl;
    TBase::hs.insert(h);
  }

  template <class THeap>
  void TestDBHP(const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < TBase::g.Size(); ++i) {
      v = Dijkstra_DBHP<THeap>(TBase::g, TBase::edge_proxy, i, TBase::max_cost);
      for (auto d : v) nhash::DCombineH(h, d);
    }
    std::cout << "Test results  [DB" << name << "]: " << h << "\t"
              << t.get_milliseconds() << std::endl;
    TBase::hs.insert(h);
  }

  template <class THeap>
  void TestDBHPW(const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < TBase::g.Size(); ++i) {
      v = Dijkstra_DBHPW<THeap>(TBase::g, TBase::edge_proxy, i,
                                TBase::max_edge_cost, TBase::max_cost);
      for (auto d : v) nhash::DCombineH(h, d);
    }
    std::cout << "Test results  [DB" << name << "]: " << h << "\t"
              << t.get_milliseconds() << std::endl;
    TBase::hs.insert(h);
  }

  template <class THeap>
  void TestDEH(const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < TBase::g.Size(); ++i) {
      v = Dijkstra_DEH<THeap>(TBase::g, TBase::edge_proxy, i, TBase::max_cost);
      for (auto d : v) nhash::DCombineH(h, d);
    }
    std::cout << "Test results  [DE" << name << "]: " << h << "\t"
              << t.get_milliseconds() << std::endl;
    TBase::hs.insert(h);
  }

  template <class THeap>
  void TestDMH(const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < TBase::g.Size(); ++i) {
      v = Dijkstra_DMH<THeap>(TBase::g, TBase::edge_proxy, i, TBase::max_cost);
      for (auto d : v) nhash::DCombineH(h, d);
    }
    std::cout << "Test results  [DM" << name << "]: " << h << "\t"
              << t.get_milliseconds() << std::endl;
    TBase::hs.insert(h);
  }

  template <class THeap>
  void TestDMHW(const std::string& name) {
    Timer t;
    size_t h = 0;
    std::vector<TEdgeCost> v;
    for (unsigned i = 0; i < TBase::g.Size(); ++i) {
      v = Dijkstra_DMHW<THeap>(TBase::g, TBase::edge_proxy, i,
                               TBase::max_edge_cost, TBase::max_cost);
      for (auto d : v) nhash::DCombineH(h, d);
    }
    std::cout << "Test results  [DM" << name << "]: " << h << "\t"
              << t.get_milliseconds() << std::endl;
    TBase::hs.insert(h);
  }

 protected:
  size_t TestDial() const;

  template <template <class TData> class THeap>
  size_t TestDBH(const std::string& name) const;

  template <class THeap>
  size_t TestDBHP(const std::string& name) const;

  template <class THeap>
  size_t TestDBHPW(const std::string& name) const;

  template <class THeap>
  size_t TestDEH(const std::string& name) const;

  template <class THeap>
  size_t TestDMH(const std::string& name) const;

  template <class THeap>
  size_t TestDMHW(const std::string& name) const;

 protected:
  void TestDijkstraHeaps() {
    if (TBase::gtype != EGraphType::LONG_PATH) {
      TestDBH<TBinaryHeap>("  BH ");
      TestDBH<TDHeap2>(" DH1 ");
      TestDBH<TDHeap4>(" DH2 ");
      TestDBH<TDHeap8>(" DH3 ");
      TestDBH<TDHeap16>(" DH4 ");
    }
    TestDEH<heap::ext::DHeapUKeyPosMap<2, TEdgeCost>>(" DH1 ");
    TestDEH<heap::ext::DHeapUKeyPosMap<4, TEdgeCost>>(" DH2 ");
    TestDEH<heap::ext::DHeapUKeyPosMap<8, TEdgeCost>>(" DH3 ");
    TestDEH<heap::ext::DHeapUKeyPosMap<16, TEdgeCost>>(" DH4 ");
    TestDMH<heap::ukvm::DHeap<2, TEdgeCost>>(" DH1 ");
    TestDMH<heap::ukvm::DHeap<4, TEdgeCost>>(" DH2 ");
    TestDMH<heap::ukvm::DHeap<8, TEdgeCost>>(" DH3 ");
    TestDMH<heap::ukvm::DHeap<16, TEdgeCost>>(" DH4 ");
    TestDMH<heap::ukvm::CompleteBinaryTree<TEdgeCost>>(" CBT ");
    TestDMH<heap::ukvm::Binomial<TEdgeCost>>(" BNML");
    TestDMH<heap::ukvm::Fibonacci<TEdgeCost>>(" FBNC");
    TestDMH<TPairing<0, 0>>(" PR00");
    TestDMH<TPairing<1, 0>>(" PR01");
    TestDMH<TPairing<0, 1>>(" PR10");
    TestDMH<TPairing<1, 1>>(" PR11");
    if (TBase::gtype != EGraphType::SPARSE)
      TestDMH<heap::ukvm::UnorderedSet<TEdgeCost>>(" USet");
  }
};
