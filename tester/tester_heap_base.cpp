#include "tester/tester_heap_base.h"

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
#include "common/memory/nodes_manager.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"

#include <functional>
#include <iostream>
#include <queue>
#include <unordered_set>

TesterHeapBase::TesterHeapBase(unsigned size_init, unsigned size_loop) {
  vinit = nvector::HRandom(size_init, 0);
  vloop = nvector::HRandom(size_loop, vinit.back());
}

size_t TesterHeapBase::TestPriorityQueue() const {
  Timer t;
  size_t h = 0;
  std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> heap(
      vinit.begin(), vinit.end());
  for (unsigned i = 0; i < vloop.size(); ++i) {
    nhash::DCombineH(h, heap.top());
    heap.pop();
    heap.push(vloop[i]);
  }
  for (; !heap.empty(); heap.pop()) nhash::DCombineH(h, heap.top());
  std::cout << "Test results [B   PQ]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapBase::TestBase(const std::string& name) const {
  Timer t;
  size_t h = 0;
  THeap heap(vinit);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    nhash::DCombineH(h, heap.Extract());
    heap.Add(vloop[i]);
  }
  for (; !heap.Empty();) nhash::DCombineH(h, heap.Extract());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapBase::TestNodesManager(const std::string& name) const {
  Timer t;
  size_t h = 0;
  typename THeap::TNodesManager nodes_manager(vinit.size());
  THeap heap(nodes_manager);
  for (size_t v : vinit) heap.Add(v);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    nhash::DCombineH(h, heap.Extract());
    heap.Add(vloop[i]);
  }
  for (; !heap.Empty();) nhash::DCombineH(h, heap.Extract());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <class THeap>
size_t TesterHeapBase::TestKVM(const std::string& name) const {
  Timer t;
  size_t h = 0;
  THeap heap(vinit.size() + vloop.size());
  for (unsigned i = 0; i < vinit.size(); ++i) heap.Set(i, vinit[i]);
  for (unsigned i = 0; i < vloop.size(); ++i) {
    nhash::DCombineH(h, heap.ExtractValue());
    heap.Set(vinit.size() + i, vloop[i]);
  }
  for (; !heap.Empty();) nhash::DCombineH(h, heap.ExtractValue());
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

template <bool multipass, bool auxiliary>
size_t TesterHeapBase::TestBasePairing() const {
  return TestBase<heap::base::Pairing<
      size_t, std::less<size_t>, memory::NodesManager, multipass, auxiliary>>(
      "B PR" + std::to_string(multipass) + std::to_string(auxiliary));
}

template <bool multipass, bool auxiliary>
size_t TesterHeapBase::TestExtPairing() const {
  return TestNodesManager<heap::ext::Pairing<
      size_t, std::less<size_t>, memory::NodesManager, multipass, auxiliary>>(
      "E PR" + std::to_string(multipass) + std::to_string(auxiliary));
}

template <bool multipass, bool auxiliary>
size_t TesterHeapBase::TestUKVMPairing() const {
  return TestKVM<
      heap::ukvm::Pairing<size_t, std::less<size_t>, multipass, auxiliary>>(
      "M PR" + std::to_string(multipass) + std::to_string(auxiliary));
}

bool TesterHeapBase::TestAll() const {
  std::unordered_set<size_t> hs;
  hs.insert(TestPriorityQueue());
  hs.insert(TestBase<heap::base::Binary<size_t>>("B   B "));
  hs.insert(TestBase<heap::base::DHeap<2, size_t>>("B   D2"));
  hs.insert(TestBase<heap::base::DHeap<4, size_t>>("B   D4"));
  hs.insert(TestBase<heap::base::DHeap<8, size_t>>("B   D8"));
  hs.insert(TestNodesManager<heap::base::Binomial<size_t>>("B BNML"));
  hs.insert(TestBasePairing<0, 0>());
  hs.insert(TestBasePairing<1, 0>());
  hs.insert(TestBasePairing<0, 1>());
  hs.insert(TestBasePairing<1, 1>());
  hs.insert(TestKVM<heap::ext::DHeapUKeyPosMap<2, size_t>>("E   D2"));
  hs.insert(TestKVM<heap::ext::DHeapUKeyPosMap<4, size_t>>("E   D4"));
  hs.insert(TestKVM<heap::ext::DHeapUKeyPosMap<8, size_t>>("E   D8"));
  hs.insert(TestNodesManager<heap::ext::Fibonacci<size_t>>("E FBNC"));
  hs.insert(TestExtPairing<0, 0>());
  hs.insert(TestExtPairing<1, 0>());
  hs.insert(TestExtPairing<0, 1>());
  hs.insert(TestExtPairing<1, 1>());
  hs.insert(TestKVM<heap::ukvm::DHeap<2, size_t>>("M   D2"));
  hs.insert(TestKVM<heap::ukvm::DHeap<4, size_t>>("M   D4"));
  hs.insert(TestKVM<heap::ukvm::DHeap<8, size_t>>("M   D8"));
  hs.insert(TestKVM<heap::ukvm::CompleteBinaryTree<size_t>>("M  CBT"));
  hs.insert(TestKVM<heap::ukvm::Binomial<size_t>>("M BNML"));
  hs.insert(TestKVM<heap::ukvm::Fibonacci<size_t>>("M FBNC"));
  hs.insert(TestUKVMPairing<0, 0>());
  hs.insert(TestUKVMPairing<1, 0>());
  hs.insert(TestUKVMPairing<0, 1>());
  hs.insert(TestUKVMPairing<1, 1>());
  return hs.size() == 1;
}

bool TestHeapBase(bool time_test) {
  TesterHeapBase th(time_test ? 10000000 : 10000, time_test ? 10000000 : 10000);
  return th.TestAll();
}
