#include "tester/tester_heap.h"

#include "common/hash.h"
#include "common/heap/binary_heap.h"
#include "common/heap/binomial.h"
#include "common/heap/binomial_ukey_value_map.h"
#include "common/heap/dheap.h"
#include "common/heap/dheap_ukey_pos_map.h"
#include "common/heap/dheap_ukey_value_map.h"
#include "common/heap/fibonacci.h"
#include "common/heap/fibonacci_ukey_value_map.h"
#include "common/heap/pairing.h"
#include "common/heap/pairing_base.h"
#include "common/heap/pairing_ukey_value_map.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"

#include <functional>
#include <iostream>
#include <queue>
#include <unordered_set>

TesterHeap::TesterHeap(unsigned size_init, unsigned size_loop) {
  vinit = nvector::HRandom(size_init, 0);
  vloop = nvector::HRandom(size_loop, vinit.back());
}

size_t TesterHeap::TestPriorityQueue() const {
  Timer t;
  size_t h = 0;
  std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> heap(
      vinit.begin(), vinit.end());
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.top());
    heap.pop();
    heap.push(vloop[i]);
  }
  for (; !heap.empty(); heap.pop()) h = HashCombine(h, heap.top());
  std::cout << "Test results [  PQ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterHeap::TestBinomialUKeyValueMap() const {
  Timer t;
  size_t h = 0;
  heap::BinomialUKeyValueMap<size_t> heap(vinit.size() + vloop.size());
  for (unsigned i = 0; i < vinit.size(); ++i) heap.Set(i, vinit[i]);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.ExtractValue());
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = HashCombine(h, heap.TopValue());
  std::cout << "Test results [BKVM]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterHeap::TestFibonacciUKeyValueMap() const {
  Timer t;
  size_t h = 0;
  heap::FibonacciUKeyValueMap<size_t> heap(vinit.size() + vloop.size());
  for (unsigned i = 0; i < vinit.size(); ++i) heap.Set(i, vinit[i]);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.ExtractValue());
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = HashCombine(h, heap.TopValue());
  std::cout << "Test results [FKVM]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeap::TestBase(const std::string& name) const {
  Timer t;
  size_t h = 0;
  THeap heap(vinit);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.Extract());
    heap.Add(vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = HashCombine(h, heap.Top());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeap::TestNodesManager(const std::string& name) const {
  Timer t;
  size_t h = 0;
  typename THeap::TNodesManager nodes_manager(vinit.size());
  THeap heap(nodes_manager);
  for (size_t v : vinit) heap.Add(v);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.Extract());
    heap.Add(vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = HashCombine(h, heap.Top());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <unsigned d>
size_t TesterHeap::TestDHeapUKeyPosMap() const {
  using THeap = heap::DHeapUKeyPosMap<d, size_t>;
  using TData = typename THeap::TData;
  std::vector<TData> vinit_adj;
  vinit_adj.reserve(vinit.size());
  for (unsigned i = 0; i < vinit.size(); ++i)
    vinit_adj.push_back({i, vinit[i]});
  Timer t;
  size_t h = 0;
  THeap heap(vinit.size() + vloop.size(), vinit_adj);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.Extract().value);
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = HashCombine(h, heap.Top().value);
  std::cout << "Test results [DKP" << d << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <unsigned d>
size_t TesterHeap::TestDHeapUKeyValueMap() const {
  using THeap = heap::DHeapUKeyValueMap<d, size_t>;
  Timer t;
  size_t h = 0;
  THeap heap(vinit.size() + vloop.size());
  for (unsigned i = 0; i < vinit.size(); ++i) heap.Set(i, vinit[i]);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.ExtractValue());
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = HashCombine(h, heap.TopValue());
  std::cout << "Test results [DKV" << d << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <bool multipass, bool auxiliary>
size_t TesterHeap::TestPairingBaseHeap() const {
  return TestBase<heap::PairingBase<size_t, std::less<size_t>, NodesManager,
                                    multipass, auxiliary>>(
      "PB" + std::to_string(multipass) + std::to_string(auxiliary));
}

template <bool multipass, bool auxiliary>
size_t TesterHeap::TestPairingHeap() const {
  return TestNodesManager<heap::Pairing<size_t, std::less<size_t>, NodesManager,
                                        multipass, auxiliary>>(
      "PR" + std::to_string(multipass) + std::to_string(auxiliary));
}

template <bool multipass, bool auxiliary>
size_t TesterHeap::TestPairingUKeyValueMap() const {
  using THeap = heap::PairingUKeyValueMap<size_t, std::less<size_t>, multipass,
                                          auxiliary>;
  Timer t;
  size_t h = 0;
  THeap heap(vinit.size() + vloop.size());
  for (unsigned i = 0; i < vinit.size(); ++i) heap.Set(i, vinit[i]);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = HashCombine(h, heap.ExtractValue());
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = HashCombine(h, heap.TopValue());
  std::cout << "Test results [PM" << auxiliary << multipass << "]: " << h
            << "\t" << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterHeap::TestAll() const {
  std::unordered_set<size_t> hs;
  hs.insert(TestPriorityQueue());
  hs.insert(TestBase<heap::BinaryHeap<size_t>>("  BH"));
  hs.insert(TestBase<heap::DHeap<2, size_t>>(" D2H"));
  hs.insert(TestBase<heap::DHeap<2, size_t>>(" D4H"));
  hs.insert(TestBase<heap::DHeap<2, size_t>>(" D8H"));
  hs.insert(TestNodesManager<heap::Binomial<size_t>>("BNML"));
  hs.insert(TestNodesManager<heap::Fibonacci<size_t>>("FBNC"));
  hs.insert(TestPairingBaseHeap<0, 0>());
  hs.insert(TestPairingBaseHeap<1, 0>());
  hs.insert(TestPairingBaseHeap<0, 1>());
  hs.insert(TestPairingBaseHeap<1, 1>());
  hs.insert(TestPairingHeap<0, 0>());
  hs.insert(TestPairingHeap<1, 0>());
  hs.insert(TestPairingHeap<0, 1>());
  hs.insert(TestPairingHeap<1, 1>());
  hs.insert(TestDHeapUKeyPosMap<2>());
  hs.insert(TestDHeapUKeyPosMap<4>());
  hs.insert(TestDHeapUKeyPosMap<8>());
  hs.insert(TestDHeapUKeyValueMap<2>());
  hs.insert(TestDHeapUKeyValueMap<4>());
  hs.insert(TestDHeapUKeyValueMap<8>());
  hs.insert(TestBinomialUKeyValueMap());
  hs.insert(TestFibonacciUKeyValueMap());
  hs.insert(TestPairingUKeyValueMap<0, 0>());
  hs.insert(TestPairingUKeyValueMap<1, 0>());
  hs.insert(TestPairingUKeyValueMap<0, 1>());
  hs.insert(TestPairingUKeyValueMap<1, 1>());
  return hs.size() == 1;
}

bool TestHeap(bool time_test) {
  TesterHeap th(time_test ? 10000000 : 10000, time_test ? 10000000 : 10000);
  return th.TestAll();
}
