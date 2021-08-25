#pragma once

#include "problem.h"
#include "solution.h"
#include "solvers/mctp_base.h"

#include <memory>
#include <string>
#include <vector>

namespace src_solvers {
// Try to find any solution that works. It stops as soon as one found.
class MCTPOne : public MCTPBase {
 public:
  using TBase = MCTPBase;

 protected:
  void InitSearch(const Problem& p);
  void Run();

 public:
  MCTPOne(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {}

  PSolver Clone() const override { return std::make_shared<MCTPOne>(*this); }

  // bool SkipSolutionRead() const override { return true; }

  std::string Name() const override {
    return "mctp_one_" + std::to_string(max_time_in_seconds);
  }

  Solution Solve(const Problem& p) override;
};
}  // namespace src_solvers
