#pragma once

#include "cache.h"
#include "problem.h"
#include "solution.h"
#include "solvers/base_vct.h"

#include "common/data_structures/unsigned_set.h"
#include "common/geometry/d2/point.h"
#include "common/solvers/solver.h"
#include "common/timer.h"

#include <memory>
#include <string>
#include <vector>

namespace src_solvers {
class MCTPBase : public BaseVCT {
 public:
  using TBase = BaseVCT;

  class Stat {
   public:
    unsigned n = 0;
    double s = 0;

    void Add(double x) {
      ++n;
      s += x;
    }

    double Score(double log_n_total, double max_score) const;
  };

 protected:
  std::vector<std::vector<Stat>> points_stats;
  std::vector<Stat> location_stats;
  unsigned nruns;
  double best_score;
  double max_score;  // For Stat::Score
  Solution best_solution;

 protected:
  void InitSearch(const Problem& p);
  void ResetSearch();
  void UpdateStat(double score);
  void Run();

 public:
  MCTPBase(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {}
};
}  // namespace src_solvers
