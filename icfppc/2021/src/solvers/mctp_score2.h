#pragma once

#include "problem.h"
#include "solution.h"
#include "solvers/mctp_base2.h"

#include <memory>
#include <string>
#include <vector>

namespace src_solvers {
class MCTPScore2 : public MCTPBase2 {
 public:
  using TBase = MCTPBase2;

 protected:
  void InitSearch(const Problem& p);
  void Run();

 public:
  MCTPScore2(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {
    use_location_stats = true;
  }

  PSolver Clone() const override { return std::make_shared<MCTPScore2>(*this); }

  // bool SkipSolutionRead() const override { return true; }

  std::string Name() const override {
    return "mctp_score2_" + std::to_string(max_time_in_seconds);
  }

  Solution Solve(const Problem& p) override;
};
}  // namespace src_solvers
