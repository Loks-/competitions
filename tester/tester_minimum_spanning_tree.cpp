#include "tester/tester_minimum_spanning_tree.h"

#include "common/graph/graph_ei/create_random_graph.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/mst/kruskal.h"
#include "common/heap/binary_heap.h"
#include "common/heap/dheap.h"
#include "common/timer.h"

#include "tester/minimum_spanning_tree.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

template <class TData>
using TBinaryHeap = heap::BinaryHeap<TData>;
template <class TData>
using TDHeap2 = heap::DHeap<2u, TData>;
template <class TData>
using TDHeap4 = heap::DHeap<4u, TData>;
template <class TData>
using TDHeap8 = heap::DHeap<8u, TData>;
template <class TData>
using TDHeap16 = heap::DHeap<16u, TData>;

TesterMinimumSpanningTree::TesterMinimumSpanningTree(EGraphType _gtype,
                                                     unsigned graph_size,
                                                     unsigned edges_per_node)
    : gtype(_gtype),
      g(CreateRandomGraph<uint64_t, false>(graph_size, edges_per_node,
                                           (1u << 30))) {}

uint64_t TesterMinimumSpanningTree::TestKruskal() const {
  Timer t;
  uint64_t d = graph::mst::Kruskal(g, edge_proxy).second;
  std::cout << "Test results Kruskal   : " << d << "\t" << t.GetMilliseconds()
            << std::endl;
  return d;
}

uint64_t TesterMinimumSpanningTree::TestPrimBaseBinaryHeap() const {
  Timer t;
  uint64_t cost =
      MinimumSpanningTreePrimBaseHeap<TBinaryHeap, TGraph, TEdgeCostFunction>(
          g, edge_proxy)
          .second;
  ;
  std::cout << "Test results Prim BBH  : " << cost << "\t"
            << t.GetMilliseconds() << std::endl;
  return cost;
}

template <template <class> class THeap>
uint64_t TesterMinimumSpanningTree::TestPrimDHeap(
    const std::string& name) const {
  Timer t;
  uint64_t cost = MinimumSpanningTreePrimHeap<THeap, TGraph, TEdgeCostFunction>(
                      g, edge_proxy, -1ull)
                      .second;
  ;
  std::cout << "Test results Prim " << name << "  : " << cost << "\t"
            << t.GetMilliseconds() << std::endl;
  return cost;
}

bool TesterMinimumSpanningTree::TestAll() {
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
  std::unordered_set<uint64_t> hs;
  if (gtype != EGraphType::DENSE) hs.insert(TestKruskal());
  if (gtype != EGraphType::DENSE) hs.insert(TestPrimBaseBinaryHeap());
  hs.insert(TestPrimDHeap<TDHeap2>("DH2"));
  hs.insert(TestPrimDHeap<TDHeap4>("DH4"));
  hs.insert(TestPrimDHeap<TDHeap8>("DH8"));
  hs.insert(TestPrimDHeap<TDHeap16>("D16"));
  return hs.size() == 1;
}

bool TestMinimumSpanningTree(bool time_test) {
  if (time_test) {
    TesterMinimumSpanningTree t1(EGraphType::SPARSE, 1000000, 10),
        t2(EGraphType::DENSE, 10000, 5000);
    return t1.TestAll() && t2.TestAll();
  } else {
    TesterMinimumSpanningTree t(EGraphType::SMALL, 100, 10);
    return t.TestAll();
  }
}
