#pragma once

#include "common/base.h"
#include "common/graph/tree_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"

#include <vector>
#include <utility>

class TesterTreePathMaxima {
 public:
  using TTree = TreeGraphEI<uint64_t>;
  using TEdgeCostFunction = graph::EdgeCostProxy<uint64_t>;

 protected:
  TEdgeCostFunction edge_proxy;
  std::vector<TTree> trees;
  std::vector<std::pair<unsigned, unsigned>> paths;

 public:
  TesterTreePathMaxima(unsigned tree_size, unsigned ntrees,
                       unsigned npaths);

 protected:
  size_t TestHLD() const;
  size_t TestHLDFBT() const;
  size_t TestPBST() const;
  size_t TestPBSTFBT() const;
  size_t TestPBST2() const;

 public:
  bool TestAll();
};
