#pragma once

#include "problem.h"
#include "solution.h"
#include "solvers/base_vct2.h"
#include "solvers/mctp_stat.h"

#include <vector>

namespace src_solvers {
// Similar to MCTPBase but use ZeroOrderOptimization at final step.
class MCTPBase2 : public BaseVCT2 {
 public:
  using TBase = BaseVCT2;

 protected:
  std::vector<std::vector<MCTPStat>> points_stats;
  bool use_location_stats;
  std::vector<MCTPStat> location_stats;
  unsigned nruns;
  double best_score;
  double exploration_mult;
  Solution best_solution;

 protected:
  void InitSearch(const Problem& p);
  void ResetSearch();
  void UpdateStat(double score);
  void RunI();
  void Run();

 public:
  MCTPBase2(unsigned max_time_in_seconds)
      : TBase(max_time_in_seconds), use_location_stats(false) {}
};
}  // namespace src_solvers
