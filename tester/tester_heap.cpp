#include "tester/tester_heap.h"

#include "common/hash.h"
#include "common/heap/binary_heap.h"
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
  std::priority_queue<size_t> heap(vinit.begin(), vinit.end());
  for (unsigned i = 0; i < vloop.size(); ++i) {
    h = hash_combine(h, heap.top());
    heap.pop();
    heap.push(vloop[i]);
  }
  for (; !heap.empty(); heap.pop()) h = hash_combine(h, heap.top());
  std::cout << "Test results [PQ]: " << h << "\t" << t.GetMilliseconds()
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
  std::cout << "Test results [BH]: " << h << "\t" << t.GetMilliseconds()
            << std::endl;
  return h;
}

bool TesterHeap::TestAll() {
  std::unordered_set<size_t> hs;
  hs.insert(TestPriorityQueue());
  hs.insert(TestBinaryHeap());
  return hs.size() == 1;
}

bool TestHeap(bool time_test) {
  TesterHeap th(time_test ? 10000000 : 10000, time_test ? 10000000 : 10000);
  return th.TestAll();
}
