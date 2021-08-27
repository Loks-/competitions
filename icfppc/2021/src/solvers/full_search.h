#pragma once

#include "problem.h"
#include "solution.h"
#include "solvers/base_vct.h"

#include "common/timer.h"

#include <memory>
#include <string>
#include <vector>

namespace src_solvers {
class FullSearch : public BaseVCT {
 public:
  using TBase = BaseVCT;

 protected:
  int64_t best_score;
  Solution best_solution;
  Timer t;

 protected:
  void InitSearch(const Problem& p);
  bool ForceStop() const;
  void Run();

 public:
  FullSearch(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {}

  PSolver Clone() const override { return std::make_shared<FullSearch>(*this); }

  // bool SkipSolutionRead() const override { return true; }

  std::string Name() const override {
    return "full_search_" + std::to_string(max_time_in_seconds);
  }

  Solution Solve(const Problem& p) override;
};
}  // namespace src_solvers
