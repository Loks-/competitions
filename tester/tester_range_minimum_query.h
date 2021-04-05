#pragma once

#include "common/base.h"
#include "common/graph/tree.h"

#include <string>
#include <utility>
#include <vector>

class RangeMinimumQuery {
 protected:
  std::vector<std::vector<size_t>> vv;
  std::vector<std::pair<unsigned, unsigned>> queries;

 public:
  RangeMinimumQuery(unsigned size, unsigned ntests, unsigned nqueries);

 protected:
  template <class TSolver>
  size_t Test(const std::string& solver_name) const;

 public:
  bool TestAll();
};
