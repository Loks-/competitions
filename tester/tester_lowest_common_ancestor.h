#pragma once

#include "common/base.h"
#include "common/graph/tree.h"

#include <utility>
#include <vector>

class TesterLowestCommonAncestor {
 protected:
  std::vector<TreeGraph> trees;
  std::vector<std::pair<unsigned, unsigned>> queries;

 public:
  TesterLowestCommonAncestor(unsigned tree_size, unsigned ntrees,
                             unsigned nqueries);

 protected:
  size_t TestTarjanOffline() const;
  size_t TestSchieberVishkin() const;
  size_t TestEulerTourRMQ() const;
  size_t TestEulerTourRMQ1() const;
  size_t TestAlphabeticCode() const;

 public:
  bool TestAll();
};
