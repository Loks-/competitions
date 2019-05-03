#pragma once

#include "common/base.h"
#include "common/graph/graph_ei.h"

#include <string>
#include <vector>

class TesterGraphEIDistancePositiveCost {
 public:
  using TGraph = graph::GraphEI<uint64_t, true>;

 protected:
  TGraph g;

 public:
  TesterGraphEIDistancePositiveCost(unsigned graph_size,
                                    unsigned edges_per_node);

 protected:
  static unsigned Size();
  std::string Name(unsigned type) const;
  size_t TestI(unsigned type);

 public:
  size_t Test(unsigned type);
  bool TestAll();
};
