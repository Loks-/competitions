#pragma once

#include "common/base.h"
#include "common/graph/graph_ei.h"

#include <string>
#include <vector>

class TesterMinimumSpanningTree {
 protected:
  graph::GraphEI<uint64_t, false> g;

 public:
  TesterMinimumSpanningTree(unsigned graph_size, unsigned edges_per_node);

 protected:
  static unsigned Size();
  std::string Name(unsigned type) const;
  uint64_t TestI(unsigned type);

 public:
  uint64_t Test(unsigned type);
  bool TestAll();
};
