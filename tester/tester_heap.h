#pragma once

#include "common/base.h"

#include <vector>

class TesterHeap {
 protected:
  std::vector<size_t> vinit, vloop;

 public:
  TesterHeap(unsigned size_init, unsigned size_loop);

  size_t TestPriorityQueue();
  size_t TestBinaryHeap();

  bool TestAll();
};
