#include "tester/tester_heap.h"

#include "common/hash.h"
#include "common/heap/binary_heap.h"
#include "common/heap/binomial.h"
#include "common/heap/dheap.h"
#include "common/heap/dheap_ukey_pos_map.h"
#include "common/heap/dheap_ukey_value_map.h"
#include "common/timer.h"

#include <iostream>
#include <queue>
#include <unordered_set>

TesterHeap::TesterHeap(unsigned size_init, unsigned size_loop) {
  size_t h = 0;
  vinit.resize(size_init);
  for (unsigned i = 0; i < size_init; ++i) {
    h = hash_combine(h, i);
    vinit[i] = h;
  }
  vloop.resize(size_loop);
  for (unsigned i = 0; i < size_loop; ++i) {
    h = hash_combine(h, i);
    vloop[i] = h;
  }
}

size_t TesterHeap::TestPriorityQueue() {
  Timer t;
  size_t h = 0;
  std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> heap(
      vinit.begin(), vinit.end());
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = hash_combine(h, heap.top());
    heap.pop();
    heap.push(vloop[i]);
  }
  for (; !heap.empty(); heap.pop()) h = hash_combine(h, heap.top());
  std::cout << "Test results [  PQ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterHeap::TestBinaryHeap() {
  Timer t;
  size_t h = 0;
  heap::BinaryHeap<size_t> heap(vinit);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = hash_combine(h, heap.Extract());
    heap.Add(vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = hash_combine(h, heap.Top());
  std::cout << "Test results [  BH]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

size_t TesterHeap::TestBinomialHeap() {
  Timer t;
  size_t h = 0;
  heap::Binomial<size_t>::TNodesManager nodes_manager(vloop.size());
  heap::Binomial<size_t> heap(nodes_manager);
  for (size_t v : vinit) heap.Add(v);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = hash_combine(h, heap.Extract());
    heap.Add(vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = hash_combine(h, heap.Top());
  std::cout << "Test results [BNML]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

template <unsigned d>
size_t TesterHeap::TestDHeap() {
  Timer t;
  size_t h = 0;
  heap::DHeap<d, size_t> heap(vinit);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = hash_combine(h, heap.Extract());
    heap.Add(vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = hash_combine(h, heap.Top());
  std::cout << "Test results [ D" << d << "H]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <unsigned d>
size_t TesterHeap::TestDHeapUKeyPosMap() {
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
    h = hash_combine(h, heap.Extract().value);
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = hash_combine(h, heap.Top().value);
  std::cout << "Test results [DKP" << d << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <unsigned d>
size_t TesterHeap::TestDHeapUKeyValueMap() {
  using THeap = heap::DHeapUKeyValueMap<d, size_t>;
  using TData = typename THeap::TData;
  Timer t;
  size_t h = 0;
  THeap heap(vinit.size() + vloop.size());
  for (unsigned i = 0; i < vinit.size(); ++i) heap.Set(i, vinit[i]);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = hash_combine(h, heap.ExtractValue());
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty(); heap.Pop()) h = hash_combine(h, heap.TopValue());
  std::cout << "Test results [DKV" << d << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterHeap::TestAll() {
  std::unordered_set<size_t> hs;
  hs.insert(TestPriorityQueue());
  hs.insert(TestBinaryHeap());
  hs.insert(TestDHeap<2>());
  hs.insert(TestDHeap<3>());
  hs.insert(TestDHeap<4>());
  hs.insert(TestDHeap<5>());
  hs.insert(TestBinomialHeap());
  hs.insert(TestDHeapUKeyPosMap<2>());
  hs.insert(TestDHeapUKeyPosMap<3>());
  hs.insert(TestDHeapUKeyPosMap<4>());
  hs.insert(TestDHeapUKeyPosMap<5>());
  hs.insert(TestDHeapUKeyValueMap<2>());
  hs.insert(TestDHeapUKeyValueMap<3>());
  hs.insert(TestDHeapUKeyValueMap<4>());
  hs.insert(TestDHeapUKeyValueMap<5>());
  return hs.size() == 1;
}

bool TestHeap(bool time_test) {
  TesterHeap th(time_test ? 1000000 : 10000, time_test ? 1000000 : 10000);
  return th.TestAll();
}
