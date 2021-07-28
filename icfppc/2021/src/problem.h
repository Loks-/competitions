#pragma once

#include "bonus.h"

#include "common/base.h"
#include "common/geometry/d2/polygon.h"
#include "common/graph/graph_ei.h"
#include "common/solvers/problem.h"

#include <string>
#include <utility>
#include <vector>

class Problem : public solvers::Problem {
 protected:
  unsigned epsilon;
  I2Polygon hole;
  UndirectedGraphEI<std::pair<int64_t, int64_t>> figure;
  std::vector<Bonus> bonuses;

 public:
  const I2Polygon& Hole() const { return hole; }

  const UndirectedGraphEI<std::pair<int64_t, int64_t>>& Figure() const {
    return figure;
  }

 protected:
  bool LoadI(const std::string& _id, const std::string& filename);

 public:
  static std::string GetFileName(const std::string& _id);
  bool Load(const std::string& _id);
};
