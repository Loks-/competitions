#pragma once

#include "problem.h"
#include "solution.h"
#include "solvers/base_vct.h"
#include "solvers/mctp_stat.h"

#include <vector>

namespace src_solvers {
class MCTPBase : public BaseVCT {
 public:
  using TBase = BaseVCT;

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
  MCTPBase(unsigned max_time_in_seconds)
      : TBase(max_time_in_seconds), use_location_stats(false) {}
};
}  // namespace src_solvers
