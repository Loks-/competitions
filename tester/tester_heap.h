#pragma once

#include "common/base.h"

#include <vector>

class TesterHeap {
 protected:
  std::vector<size_t> vinit, vloop;

 public:
  TesterHeap(unsigned size_init, unsigned size_loop);

 protected:
  size_t TestPriorityQueue();
  size_t TestBinaryHeap();
  size_t TestBinomialHeap();
  size_t TestUKeyValueHeap();
  size_t TestUKeyValueMap();

  template <unsigned d>
  size_t TestDHeap();

  template <unsigned d>
  size_t TestDHeapUKeyValueMap();

 public:
  bool TestAll();
};
