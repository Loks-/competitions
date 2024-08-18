#include "tester/tester_heap_ext.h"

#include "common/hash/combine.h"
#include "common/heap/base/binary.h"
#include "common/heap/base/binomial.h"
#include "common/heap/base/dheap.h"
#include "common/heap/base/pairing.h"
#include "common/heap/ext/dheap_ukey_pos_map.h"
#include "common/heap/ext/fibonacci.h"
#include "common/heap/ext/pairing.h"
#include "common/heap/ukvm/binomial.h"
#include "common/heap/ukvm/complete_binary_tree.h"
#include "common/heap/ukvm/dheap.h"
#include "common/heap/ukvm/fibonacci.h"
#include "common/heap/ukvm/pairing.h"
#include "common/heap/ukvm/proxy_set.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>

using TValue = size_t;
using TPair = std::pair<TValue, unsigned>;

TesterHeapExt::TesterHeapExt(unsigned _size, unsigned _dpm)
    : size(_size), dpm(_dpm), vinf(_size, size_t(-1ll)) {
  v = nvector::HRandom(size * dpm, 0);
  for (unsigned i = 0; i < size; ++i) {
    auto itb = v.begin() + i * dpm, ite = itb + dpm;
    std::sort(itb, ite);
    std::reverse(itb, ite);
  }
}

bool TesterHeapExt::CheckHash() const { return hs.size() == 1; }

size_t TesterHeapExt::TestPriorityQueue() const {
  Timer t;
  size_t h = 0;
  std::priority_queue<TPair, std::vector<TPair>, std::greater<TPair>> heap;
  for (unsigned j = 0; j < dpm; ++j) {
    for (unsigned i = 0; i < size; ++i)
      heap.push(std::make_pair(v[i * dpm + j], i));
    nhash::DCombineH(h, heap.top().first);
  }
  std::vector<unsigned> in_heap(size, 1);
  for (; !heap.empty();) {
    auto v = heap.top();
    heap.pop();
    if (in_heap[v.second]) {
      nhash::DCombineH(h, v.first);
      in_heap[v.second] = 0;
    }
  }
  std::cout << "Test results [B   PQ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapExt::TestBase(const std::string& name) const {
  Timer t;
  size_t h = 0;
  THeap heap(size * dpm);
  for (unsigned j = 0; j < dpm; ++j) {
    for (unsigned i = 0; i < size; ++i)
      heap.Add(std::make_pair(v[i * dpm + j], i));
    nhash::DCombineH(h, heap.Top().first);
  }
  std::vector<unsigned> in_heap(size, 1);
  for (; !heap.Empty();) {
    auto v = heap.Extract();
    if (in_heap[v.second]) {
      nhash::DCombineH(h, v.first);
      in_heap[v.second] = 0;
    }
  }
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapExt::TestBaseNodesManager(const std::string& name) const {
  Timer t;
  size_t h = 0;
  typename THeap::TNodesManager nodes_manager(size * dpm);
  THeap heap(nodes_manager);
  for (unsigned j = 0; j < dpm; ++j) {
    for (unsigned i = 0; i < size; ++i)
      heap.Add(std::make_pair(v[i * dpm + j], i));
    nhash::DCombineH(h, heap.Top().first);
  }
  std::vector<unsigned> in_heap(size, 1);
  for (; !heap.Empty();) {
    auto v = heap.Extract();
    if (in_heap[v.second]) {
      nhash::DCombineH(h, v.first);
      in_heap[v.second] = 0;
    }
  }
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapExt::TestExt(const std::string& name) const {
  Timer t;
  size_t h = 0;
  THeap heap(size);
  for (unsigned j = 0; j < dpm; ++j) {
    for (unsigned i = 0; i < size; ++i)
      heap.DecreaseValueIfLess(i, v[i * dpm + j]);
    nhash::DCombineH(h, heap.TopValue());
  }
  for (; !heap.Empty();) nhash::DCombineH(h, heap.ExtractValue());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapExt::TestExtNodesManager(const std::string& name) const {
  Timer t;
  size_t h = 0;
  typename THeap::TNodesManager nodes_manager(size);
  THeap heap(nodes_manager);
  for (unsigned i = 0; i < size; ++i) heap.Add(vinf[i]);
  for (unsigned j = 0; j < dpm; ++j) {
    for (unsigned i = 0; i < size; ++i)
      heap.DecreaseValueIfLess(nodes_manager.NodeByRawIndex(i), v[i * dpm + j]);
    nhash::DCombineH(h, heap.Top());
  }
  for (; !heap.Empty();) nhash::DCombineH(h, heap.Extract());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapExt::TestKVM(const std::string& name) const {
  Timer t;
  size_t h = 0;
  THeap heap(vinf, false);
  for (unsigned j = 0; j < dpm; ++j) {
    for (unsigned i = 0; i < size; ++i)
      heap.DecreaseValueIfLess(i, v[i * dpm + j]);
    nhash::DCombineH(h, heap.TopValue());
  }
  for (; !heap.Empty();) nhash::DCombineH(h, heap.ExtractValue());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <bool multipass, bool auxiliary>
size_t TesterHeapExt::TestBasePairing() const {
  return TestBase<heap::base::Pairing<
      TPair, std::less<TPair>, memory::NodesManager, multipass, auxiliary>>(
      "B PR" + std::to_string(multipass) + std::to_string(auxiliary));
}

template <bool multipass, bool auxiliary>
size_t TesterHeapExt::TestExtPairing() const {
  return TestExtNodesManager<heap::ext::Pairing<
      size_t, std::less<size_t>, memory::NodesManager, multipass, auxiliary>>(
      "E PR" + std::to_string(multipass) + std::to_string(auxiliary));
}

template <bool multipass, bool auxiliary>
size_t TesterHeapExt::TestUKVMPairing() const {
  return TestKVM<
      heap::ukvm::Pairing<TValue, std::less<TValue>, multipass, auxiliary>>(
      "M PR" + std::to_string(multipass) + std::to_string(auxiliary));
}

template <class THeap, class TExtra>
size_t TesterHeapExt::TestKVME(const std::string& name, const TExtra& e) const {
  Timer t;
  size_t h = 0;
  THeap heap(e, vinf, false);
  for (unsigned j = 0; j < dpm; ++j) {
    for (unsigned i = 0; i < size; ++i)
      heap.DecreaseValueIfLess(i, v[i * dpm + j]);
    nhash::DCombineH(h, heap.TopValue());
  }
  for (; !heap.Empty();) nhash::DCombineH(h, heap.ExtractValue());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterHeapExt::TestAll() {
  bool small = (size * dpm < 10000000);
  if (small) {
    hs.insert(TestPriorityQueue());
    hs.insert(TestBase<heap::base::Binary<TPair>>("B   B "));
    hs.insert(TestBase<heap::base::DHeap<2, TPair>>("B   D2"));
    hs.insert(TestBase<heap::base::DHeap<4, TPair>>("B   D4"));
    hs.insert(TestBase<heap::base::DHeap<8, TPair>>("B   D8"));
    hs.insert(TestBaseNodesManager<heap::base::Binomial<TPair>>("B BNML"));
    hs.insert(TestBasePairing<0, 0>());
    hs.insert(TestBasePairing<1, 0>());
    hs.insert(TestBasePairing<0, 1>());
    hs.insert(TestBasePairing<1, 1>());
  }
  hs.insert(TestExt<heap::ext::DHeapUKeyPosMap<2, size_t>>("E   D2"));
  hs.insert(TestExt<heap::ext::DHeapUKeyPosMap<4, size_t>>("E   D4"));
  hs.insert(TestExt<heap::ext::DHeapUKeyPosMap<8, size_t>>("E   D8"));
  hs.insert(TestExtNodesManager<heap::ext::Fibonacci<size_t>>("E FBNC"));
  hs.insert(TestExtPairing<0, 0>());
  hs.insert(TestExtPairing<1, 0>());
  hs.insert(TestExtPairing<0, 1>());
  hs.insert(TestExtPairing<1, 1>());
  hs.insert(TestKVM<heap::ukvm::DHeap<2, TValue>>("M   D2"));
  hs.insert(TestKVM<heap::ukvm::DHeap<4, TValue>>("M   D4"));
  hs.insert(TestKVM<heap::ukvm::DHeap<8, TValue>>("M   D8"));
  hs.insert(TestKVM<heap::ukvm::CompleteBinaryTree<TValue>>("M  CBT"));
  hs.insert(TestKVM<heap::ukvm::Binomial<TValue>>("M BNML"));
  hs.insert(TestKVM<heap::ukvm::Fibonacci<TValue>>("M FBNC"));
  hs.insert(TestUKVMPairing<0, 0>());
  hs.insert(TestUKVMPairing<1, 0>());
  hs.insert(TestUKVMPairing<0, 1>());
  hs.insert(TestUKVMPairing<1, 1>());
  return CheckHash();
}

bool TestHeapExt(bool time_test) {
  TesterHeapExt th(time_test ? 10000 : 100, time_test ? 10000 : 100);
  return th.TestAll();
}
